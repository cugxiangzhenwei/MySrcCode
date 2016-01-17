#include <stdio.h>
#include <stdlib.h>

#include "math.h"

#include "mpi.h"

/*bool ReadColorBmp(char *name, unsigned char *&body, unsigned char *&head, int &length)
{
	FILE *fp=fopen(name,"rb");
	if(fp==NULL)
	{
		perror("Open file failure");
		return false;
	}

	head=(unsigned char*)malloc(sizeof(unsigned char)*54);
	fread(head,sizeof(unsigned char),54,fp);	
	unsigned int OffBits=(head[10]&0x000000ff)+((head[11]&0x000000ff)<<8)+((head[12]&0x000000ff)<<16)+((head[13]&0x000000ff)<<24);
	unsigned int width=(head[18]&0x000000ff)+((head[19]&0x000000ff)<<8)+((head[20]&0x000000ff)<<16)+((head[21]&0x000000ff)<<24);
	unsigned int height=(head[22]&0x000000ff)+((head[23]&0x000000ff)<<8)+((head[24]&0x000000ff)<<16)+((head[25]&0x000000ff)<<24);
	
	length=width*height;
	fseek(fp, OffBits, SEEK_SET);	
	body=(unsigned char*)malloc(sizeof(unsigned char)*length*3);	
	fread(body, sizeof(unsigned char),length*3,fp);
	
	fclose(fp);

	return true;
} */


//bool Convert(unsigned char *&pColor, unsigned char *&pGray, int length)
bool Convert(unsigned char *&pColor, unsigned char *&pGray,double *&pIHS, int length)
{
    length=length*3;
    for (int i=0;i<length;)
    {
        double B=pColor[i++];
        double G=pColor[i++];
        double R=pColor[i++];
        
        int position=(i-3)/3;
        
        double I,H,S,min;
        
        min=B<G?(R<B?R:B):(R<G?R:G);
        
        I=(B+G+R)/3;
        if(min==B)
        {
            H=(G-B)/(3*(I-B));
            S=1.0-(B/I);
        //    I=pGray[position];
            
           /* B=(1.0-S)*I;
            G=3*H*I+(1.0-3*H)*B;
            R=3*I-B-G; */
        }

        else if(min==R)
        {
            H=(B-R)/(3*(I-R));
            S=1.0-(R/I);
           // I=pGray[position];
            
         /*   R=(1.0-S)*I;
            B=3*H*I+(1.0-3*H)*R;
            G=3*I-R-B; */
        }
        
        else{
            H=(R-G)/(3*(I-G));
            S=1.0-(G/I);
          //  I=pGray[position];
            
           /* G=(1.0-S)*I;
            R=3*H*I+(1.0-3*H)*G;
            B=3*I-R-G; */           
        }
      

	/*	pColor[i-3]=(unsigned char)I;
        pColor[i-2]=(unsigned char)H;
        pColor[i-1]=(unsigned char)S; */

		pIHS[i-3]=I;
        pIHS[i-2]=H;
        pIHS[i-1]=S; 


    }
    return true;
}


bool ReadColorBmp(char *name, unsigned char *&body, unsigned char *&head, int &height,int &width,int &length)
//bool ReadColorBmp(char *name, unsigned char *&body, unsigned char *&head,int height,int width, int &length)
{
    FILE *fp=fopen(name,"rb");
    if(fp==NULL)
    {
        perror("Open file failure");
        return false;
    }

    head=(unsigned char*)malloc(sizeof(unsigned char)*54);
    fread(head,sizeof(unsigned char),54,fp);    
    unsigned int OffBits=(head[10]&0x000000ff)+((head[11]&0x000000ff)<<8)+((head[12]&0x000000ff)<<16)+((head[13]&0x000000ff)<<24);
	width=(head[18]&0x000000ff)+((head[19]&0x000000ff)<<8)+((head[20]&0x000000ff)<<16)+((head[21]&0x000000ff)<<24);
	height=(head[22]&0x000000ff)+((head[23]&0x000000ff)<<8)+((head[24]&0x000000ff)<<16)+((head[25]&0x000000ff)<<24);
    
    length=width*height;
    fseek(fp, OffBits, SEEK_SET);    
    body=(unsigned char*)malloc(sizeof(unsigned char)*length*3);    
    fread(body, sizeof(unsigned char),length*3,fp);
    
    fclose(fp);
	
    return true;
}



bool WriteColorBmp(char *name, unsigned char *&body, unsigned char *&head, int length)
{
    FILE *fp=fopen(name,"wb");
    if(fp==NULL)
    {
        perror("Open file failure");
        return false;
    }
    
    fwrite(head, sizeof(unsigned char),54,fp);
    fwrite(body, sizeof(unsigned char),length*3,fp);
    fclose(fp);
    return true;
}

bool ReadGrayBmp(char *name, unsigned char *&body, int &length)
{
    FILE *fp=fopen(name,"rb");
    if(fp==NULL)
    {
        perror("Open file failure");
        return false;
    }
    
    unsigned char head[54];
    fread(head,sizeof(unsigned char),54,fp);    
    unsigned int OffBits=(head[10]&0x000000ff)+((head[11]&0x000000ff)<<8)+((head[12]&0x000000ff)<<16)+((head[13]&0x000000ff)<<24);
    unsigned int width=(head[18]&0x000000ff)+((head[19]&0x000000ff)<<8)+((head[20]&0x000000ff)<<16)+((head[21]&0x000000ff)<<24);
    unsigned int height=(head[22]&0x000000ff)+((head[23]&0x000000ff)<<8)+((head[24]&0x000000ff)<<16)+((head[25]&0x000000ff)<<24);

    length=width*height;
    fseek(fp, OffBits, SEEK_SET);    
    body=(unsigned char*)malloc(sizeof(unsigned char)*length);    
    fread(body, sizeof(unsigned char),length,fp);
    
    fclose(fp);
    return true;
}


/////////////////////////////////////////////////////////////////////////////////////
//////////////////////                 下面是计算清晰度G                            %
///////////////////////////////////////////////////////////////////////////////////// 
bool ComputedClarity(unsigned char *&pColor, double *&clarity, int height,int width)
{
    double *temp,*Fr,*Fg,*Fb;
	int length;
	temp=(double*)malloc(sizeof(double)*3);
	clarity=(double*)malloc(sizeof(double)*3);

	Fr=(double*)malloc(sizeof(double)*height*width);
    Fg=(double*)malloc(sizeof(double)*height*width);
	Fb=(double*)malloc(sizeof(double)*height*width);


    for(int i=0;i<3;i++)
	{
		clarity[i] = 0;
		temp[i] = 0;
	}

  length=height*width;
  int k=0;

	for(int i=0;i<3*length;)
	{	    
		Fb[k]=pColor[i++];
		Fg[k]=pColor[i++];
		Fr[k]=pColor[i++];
		k++;		
	}

	double r1,r2,g1,g2,b1,b2;

	double SR,SG,SB;
	
 for(int i=0;i<height-1;i++)
   for(int j=0;j<width-1;j++)
   {
	   r1=Fr[(i+1)*width+j]-Fr[i*width+j];
	   r2=Fr[i*width+j+1]-Fr[i*width+j];
	   g1=Fg[(i+1)*width+j]-Fg[i*width+j];
	   g2=Fg[i*width+j+1]-Fg[i*width+j];
	   b1=Fb[(i+1)*width+j]-Fb[i*width+j];
	   b2=Fb[i*width+j+1]-Fb[i*width+j];

	   SR=r1*r1+r2*r2;
	   SG=(g1*g1+g2*g2);
	   SB=(b1*b1+b2*b2);

	   
	    temp[0]=temp[0]+sqrt(SR/2);
        temp[1]=temp[1]+sqrt(SG/2);
        temp[2]=temp[2]+sqrt(SB/2);
   }


	for(int i=0;i<3;i++)
	{
	    clarity[i]=temp[i]/((height-1)*(width-1));
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*%               %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%               %                       下面是标准差                            %
%               %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  */
bool ComputedStandardCha(unsigned char *&pColor, double *&StandardCha, int height,int width)
{
    double *temp,*Fr,*Fg,*Fb;
	int length;
	temp=(double*)malloc(sizeof(double)*3);
	StandardCha=(double*)malloc(sizeof(double)*3);

	Fr=(double*)malloc(sizeof(double)*height*width);
    Fg=(double*)malloc(sizeof(double)*height*width);
	Fb=(double*)malloc(sizeof(double)*height*width);


    for(int i=0;i<3;i++)
	{
	   StandardCha[i] = 0;
		temp[i] = 0;
	}

  length=height*width;
  int k=0;

		for(int i=0;i<3*length;)
	{	    
		Fb[k]=pColor[i++];
		Fg[k]=pColor[i++];
		Fr[k]=pColor[i++];
		k++;		
	}

	//double r1,r2,g1,g2,b1,b2;
 
    double r1,g1,b1;
	double meanB,meanG,meanR;
	meanB=0; meanG=0; meanR=0;

	for(int i=0;i<length;i++)
	{	    
	    meanB=meanB+Fb[i];
        meanG=meanG+Fg[i];
        meanR=meanR+Fr[i];
			
	}

meanB=meanB/length; meanG=meanG/length; meanR=meanR/length;

double fenmu_up_R=0;
double fenmu_up_G=0;
double fenmu_up_B=0;  


	
 for(int i=0;i<height;i++)
   for(int j=0;j<width;j++)
   {
	   r1=Fr[i*width+j];
	   
	   g1=Fg[i*width+j];
	  
	   b1=Fb[i*width+j];
	   

	   fenmu_up_R=fenmu_up_R+(r1-meanB)*(r1-meanB);
       fenmu_up_G=fenmu_up_G+(g1-meanG)*(g1-meanG);
	   fenmu_up_B=fenmu_up_B+(b1-meanR)*(b1-meanR); 
	   
   }

   StandardCha[0]= sqrt(fenmu_up_R/(length-1));
   StandardCha[1]= sqrt(fenmu_up_G/(length-1));
   StandardCha[2]= sqrt(fenmu_up_B/(length-1));  

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
           /*   %                       下面是偏差指数　                         % */
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ComputeDeviationIndex(unsigned char *&pColorFusion,unsigned char *&pColorLow, double *&DeviationIndex, int height,int width)
{
    double *temp,*Fr,*Fg,*Fb;
	double *Lr,*Lg,*Lb;
	int length;
	temp=(double*)malloc(sizeof(double)*3);
	DeviationIndex=(double*)malloc(sizeof(double)*3);

	Fr=(double*)malloc(sizeof(double)*height*width);
    Fg=(double*)malloc(sizeof(double)*height*width);
	Fb=(double*)malloc(sizeof(double)*height*width);

	Lr=(double*)malloc(sizeof(double)*height*width);
    Lg=(double*)malloc(sizeof(double)*height*width);
	Lb=(double*)malloc(sizeof(double)*height*width);


    for(int i=0;i<3;i++)
	{
		DeviationIndex[i] = 0;
		temp[i] = 0;
	}

  length=height*width;
  int k=0;

for(int i=0;i<3*length;)
	{	    
		Fb[k]=pColorFusion[i++];
		Fg[k]=pColorFusion[i++];
		Fr[k]=pColorFusion[i++];
		k++;		
	}

k=0;
for(int i=0;i<3*length;)
	{	
		
		Lb[k]=pColorLow[i++];
		Lg[k]=pColorLow[i++];
		Lr[k]=pColorLow[i++];

		k++;		
	}

	double r1,r2,g1,g2,b1,b2;
	
 for(int i=0;i<height;i++)
   for(int j=0;j<width;j++)
   {
	   r1=Fr[i*width+j];
	   r2=Lr[i*width+j];
	   
	   g1=Fg[i*width+j];
	   g2=Lg[i*width+j];

	   b1=Fb[i*width+j];
	   b2=Lb[i*width+j];	   
	   
	   if (r2!=0)
	   temp[0]=temp[0]+abs(r1-r2)/r2;
	   if (g2!=0)
	   temp[1]=temp[1]+abs(g1-g2)/g2;
       if (b2!=0)
	   temp[2]=temp[2]+abs(b1-b2)/b2;

   }


	for(int i=0;i<3;i++)
	{
	    DeviationIndex[i]=temp[i]/length;
	}

	return true;
}


///////////////////////////////////////////////////////
///////             %                       下面是计算相关系数                           %
///////////////////////////////////////////////////////
//bool ComputeCorrelationXS(unsigned char *&pColorFusion, unsigned char *&pColorLow,double *&CorrelationXS, int height,int width)
//bool ComputeCorrelationXS(unsigned char *&pColorFusion, unsigned char *&pColorLow,double *&meanFRGB,double *&meanLRGB,double *&fenZiMuUpLowRGB, int height,int width)
bool ComputeCorrelationXS(unsigned char *&pColorFusion, unsigned char *&pColorLow,double *&meanFRGB,double *&meanLRGB,double *&fenZiMuUpLowRGB, int length)
{
    
	double *temp,*Fr,*Fg,*Fb;
	double *Lr,*Lg,*Lb;
	//int length;
	temp=(double*)malloc(sizeof(double)*3);
//	CorrelationXS=(double*)malloc(sizeof(double)*3);
	fenZiMuUpLowRGB=(double*)malloc(sizeof(double)*9);

	Fr=(double*)malloc(sizeof(double)*length);
    Fg=(double*)malloc(sizeof(double)*length);
	Fb=(double*)malloc(sizeof(double)*length);

	Lr=(double*)malloc(sizeof(double)*length);
    Lg=(double*)malloc(sizeof(double)*length);
	Lb=(double*)malloc(sizeof(double)*length);


    for(int i=0;i<3;i++)
	{
	//	CorrelationXS[i] = 0;
		temp[i] = 0;
	}

	for(int i=0;i<9;i++)
	{
	//	CorrelationXS[i] = 0;
		fenZiMuUpLowRGB[i] = 0;
	}

  //length=height*width;
int   k=0;
  for(int i=0;i<3*length;)
	{	    
		Fb[k]=pColorFusion[i++];
		Fg[k]=pColorFusion[i++];
		Fr[k]=pColorFusion[i++];
		k++;		
	}

k=0;
for(int i=0;i<3*length;)
	{	
		
		Lb[k]=pColorLow[i++];
		Lg[k]=pColorLow[i++];
		Lr[k]=pColorLow[i++];

		k++;		
	}


	double r1,r2,g1,g2,b1,b2;
	double meanFB,meanFG,meanFR;
    double meanLB,meanLG,meanLR;
	
/*	meanFB=0; meanFG=0; meanFR=0;
	meanLB=0; meanLG=0; meanLR=0;

	k=0; 

	for(int i=0;i<length;i++)
	{	    

        meanFB=meanFB+Fb[i];
        meanFG=meanFG+Fg[i];
        meanFR=meanFR+Fr[i];



        meanLB=meanLB+Lb[i];
        meanLG=meanLG+Lg[i];
        meanLR=meanLR+Lr[i];

	//	k++;		
	}

	meanFB=meanFB/length; meanFG=meanFG/length; meanFR=meanFR/length;
	
	meanLB=meanLB/length; meanLG=meanLG/length; meanLR=meanLR/length;
*/
	meanFB=meanFRGB[0]; meanFG=meanFRGB[1]; meanFR=meanFRGB[2];
	
	meanLB=meanLRGB[0]; meanLG=meanLRGB[1]; meanLR=meanLRGB[2];
	
/*
	double fenzi_R,fenmu_up_R,fenmu_low_R;

	double fenzi_G,fenmu_up_G,fenmu_low_G;

	double fenzi_B,fenmu_up_B,fenmu_low_B; 

	double rou_R,rou_G,rou_B;

	fenzi_R=0;fenmu_up_R=0;fenmu_low_R=0;
	
    fenzi_G=0;fenmu_up_G=0;fenmu_low_G=0;
	
	fenzi_B=0;fenmu_up_B=0;fenmu_low_B=0;  */

	
 //for(int i=0;i<height-1;i++)
 //  for(int j=0;j<width-1;j++)

for(int i=0;i<length;i++)
   {
	  /* r1=Fr[i*width+j];
	   r2=Lr[i*width+j];
	   g1=Fg[i*width+j];
	   g2=Lg[i*width+j];
	   b1=Fb[i*width+j];
	   b2=Lb[i*width+j]; */	  
	   
	   r1=Fr[i];
	   r2=Lr[i];
	   g1=Fg[i];
	   g2=Lg[i];
	   b1=Fb[i];
	   b2=Lb[i];

      /* fenzi_R=fenzi_R+(r1-meanFR)*(r2-meanLR);
        fenmu_up_R=fenmu_up_R+(r1-meanFR)*(r1-meanFR);
        fenmu_low_R=fenmu_low_R+(r2-meanLR)*(r2-meanLR);
        
        fenzi_G=fenzi_G+(g1-meanFG)*(g2-meanLG);
        fenmu_up_G=fenmu_up_G+(g1-meanFG)*(g1-meanFG);
        fenmu_low_G=fenmu_low_G+(g2-meanLG)*(g2-meanLG);
        
        fenzi_B=fenzi_B+(b1-meanFB)*(b2-meanLB);
        fenmu_up_B=fenmu_up_B+(b1-meanFB)*(b1-meanFB);
        fenmu_low_B=fenmu_low_B+(b2-meanLB)*(b2-meanLB);  */

	   fenZiMuUpLowRGB[0]=fenZiMuUpLowRGB[0]+(r1-meanFRGB[2])*(r2-meanLRGB[2]);
       fenZiMuUpLowRGB[1]=fenZiMuUpLowRGB[1]+(r1-meanFRGB[2])*(r1-meanFRGB[2]);
       fenZiMuUpLowRGB[2]=fenZiMuUpLowRGB[2]+(r2-meanLRGB[2])*(r2-meanLRGB[2]);
        
        fenZiMuUpLowRGB[3]=fenZiMuUpLowRGB[3]+(g1-meanFRGB[1])*(g2-meanLRGB[1]);
        fenZiMuUpLowRGB[4]=fenZiMuUpLowRGB[4]+(g1-meanFRGB[1])*(g1-meanFRGB[1]);
        fenZiMuUpLowRGB[5]=fenZiMuUpLowRGB[5]+(g2-meanLRGB[1])*(g2-meanLRGB[1]);
        
        fenZiMuUpLowRGB[6]=fenZiMuUpLowRGB[6]+(b1-meanFRGB[0])*(b2-meanLRGB[0]);
        fenZiMuUpLowRGB[7]=fenZiMuUpLowRGB[7]+(b1-meanFRGB[0])*(b1-meanFRGB[0]);
        fenZiMuUpLowRGB[8]=fenZiMuUpLowRGB[8]+(b2-meanLRGB[0])*(b2-meanLRGB[0]);

   }
	
 /*  CorrelationXS[0]=fenzi_R/(sqrt(fenmu_up_R*fenmu_low_R));
   CorrelationXS[1]=fenzi_G/(sqrt(fenmu_up_G*fenmu_low_G));
   CorrelationXS[2]=fenzi_B/(sqrt(fenmu_up_B*fenmu_low_B)); */

	return true;
}
 
 bool AddColor(unsigned char *&pColor, double*&RGB, int length)
{   
	RGB=(double*)malloc(sizeof(double)*3);

    for(int i=0;i<3;i++)
	{	   
		RGB[i] = 0;
	}

	for(int i=0;i<length*3;)
	{	    
		double B=pColor[i++];
		double G=pColor[i++];
		double R=pColor[i++];

		RGB[0]+=B;	//sum B
		RGB[1]+=G;	//sum G
		RGB[2]+=R;	//sum R
	}

	return true;        
}   
             



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	int myid, numprocs;
	double starttime, endtime;	
	MPI_Status s;

	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);	
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);	
	starttime = MPI_Wtime(); 	
	//F-Fusion, L-Low;
	unsigned char *colorData;

	int length=0;	
	
	unsigned char* colorBodyF,*colorBodyL;

	unsigned char* colorDataF,* colorDataL;

	unsigned char* colorHeadF, * colorHeadL;

	int HeightFusion,WidthFusion,LengthFusion;

	int HeightLow,WidthLow,LengthLow;

//	int length;
	int height,width;

	double *RGBF,*RGBL;      
    double  *colorBodyNew;	
	double RGBNewF[3];
	double RGBNewL[3];
  //  int colorLength, grayLength;
	int colorLength;
//	double eps = 0.000001;   //控制精度要求

  //  RGBF=(double*)malloc(sizeof(double)*3);
//	RGBL=(double*)malloc(sizeof(double)*3);
	
	
	printf("**********************************\n");

	fprintf(stdout,"Process %d of %d on %s\n",myid,numprocs,processor_name);

    if (myid==0)
	{
	
       	unsigned char* colorBody;	
		
	    ReadColorBmp("C:\\XXR_Image\\color.bmp", colorBodyF, colorHeadF,height,width, colorLength);

		ReadColorBmp("C:\\XXR_Image\\tm.bmp", colorBodyL, colorHeadL, height,width,colorLength);

		printf("colorLength=%d\n",colorLength);        

		while ((colorLength%numprocs)!=0)
		{
			numprocs--;
		}
		
		length=colorLength/numprocs;
		colorDataF=colorBodyF;
		colorDataL=colorBodyL;

		
		for (int i=1;i<numprocs;i++)
		{
			MPI_Send(&length,1,MPI_INT,i,20,MPI_COMM_WORLD);
			MPI_Send(colorBodyF+i*length*3,length*3,MPI_UNSIGNED_CHAR,i,21,MPI_COMM_WORLD);
		    MPI_Send(colorBodyL+i*length*3,length*3,MPI_UNSIGNED_CHAR,i,22,MPI_COMM_WORLD);
		}	
		
	}

	else
	{
		MPI_Status s;
		MPI_Recv(&length,1,MPI_INT,0,20,MPI_COMM_WORLD,&s);
		colorDataF=(unsigned char* )malloc(sizeof(unsigned char)*length*3);
        colorDataL=(unsigned char* )malloc(sizeof(unsigned char)*length*3);
		MPI_Recv(colorDataF,length*3,MPI_UNSIGNED_CHAR,0,21,MPI_COMM_WORLD,&s);
		MPI_Recv(colorDataL,length*3,MPI_UNSIGNED_CHAR,0,22,MPI_COMM_WORLD,&s);
	}
        	
    //printf("colorLength=%d\n",colorLength);
      printf("length=%d\n",length);     
	  AddColor(colorDataF, RGBF, length); 
	  AddColor(colorDataL, RGBL, length); 

		if (myid==0)
	{				
	    double *RGBDataF;   
        RGBDataF=(double*)malloc(sizeof(double)*3*numprocs);	
        MPI_Send(RGBF,3,MPI_DOUBLE,0,30,MPI_COMM_WORLD);
		MPI_Recv(RGBDataF,3,MPI_DOUBLE,0,30,MPI_COMM_WORLD,&s);

        double *RGBDataL;
        RGBDataL=(double*)malloc(sizeof(double)*3*numprocs);	
        MPI_Send(RGBL,3,MPI_DOUBLE,0,31,MPI_COMM_WORLD);
		MPI_Recv(RGBDataL,3,MPI_DOUBLE,0,31,MPI_COMM_WORLD,&s);

		
		for (int i=1;i<numprocs;i++)
		{
		//	MPI_Status s;
			MPI_Recv(RGBDataF+3*i,3,MPI_DOUBLE,i,33,MPI_COMM_WORLD,&s);
			MPI_Recv(RGBDataL+3*i,3,MPI_DOUBLE,i,34,MPI_COMM_WORLD,&s);
		}

		double temp0,temp1,temp2;
		temp0=0;
	    temp1=0;
	    temp2=0;

		for(int i=0;i<3;i++)
		{	   
		  RGBNewF[i]=0;
          RGBNewL[i]=0;
		}        
     
        for (int i=0;i<numprocs*3;)
		{         
		 temp0=RGBDataF[i++];           
         temp1=RGBDataF[i++];
         temp2=RGBDataF[i++];
           
	     RGBNewF[0]+=temp0;
		 RGBNewF[1]+=temp1;
		 RGBNewF[2]+=temp2;
		}

		RGBNewF[0]=RGBNewF[0]/colorLength;
        RGBNewF[1]=RGBNewF[1]/colorLength;
		RGBNewF[2]=RGBNewF[2]/colorLength; 

     	temp0=0;
	    temp1=0;
	    temp2=0;
		
        for (int i=0;i<numprocs*3;)
		{         
		 temp0=RGBDataL[i++];           
         temp1=RGBDataL[i++];
         temp2=RGBDataL[i++];
           
	     RGBNewL[0]+=temp0;
		 RGBNewL[1]+=temp1;
		 RGBNewL[2]+=temp2;
		}

      RGBNewL[0]=RGBNewL[0]/colorLength;
      RGBNewL[1]=RGBNewL[1]/colorLength;
      RGBNewL[2]=RGBNewL[2]/colorLength; 

      printf("RGBNewF[0]=%f,RGBNewF[1]=%f,RGBNewF[2]=%f\n\n",RGBNewF[0],RGBNewF[1],RGBNewF[2]);
	  printf("RGBNewL[0]=%f,RGBNewL[1]=%f,RGBNewL[2]=%f\n\n",RGBNewL[0],RGBNewL[1],RGBNewL[2]); 
     
	} 

	else
	{ 	
		MPI_Send(RGBF,3,MPI_DOUBLE,0,33,MPI_COMM_WORLD);
		MPI_Send(RGBL,3,MPI_DOUBLE,0,34,MPI_COMM_WORLD);
	}/**/

	 double *meanFRGB;
     meanFRGB=(double*)malloc(sizeof(double)*3);

	 double *meanLRGB;
     meanLRGB=(double*)malloc(sizeof(double)*3);

    if (myid==0)
	{				
      MPI_Send(&RGBNewL,3,MPI_DOUBLE,0,40,MPI_COMM_WORLD);
	  MPI_Recv(meanFRGB,3,MPI_DOUBLE,0,40,MPI_COMM_WORLD,&s);
	  MPI_Send(&RGBNewF,3,MPI_DOUBLE,0,41,MPI_COMM_WORLD);
	  MPI_Recv(meanLRGB,3,MPI_DOUBLE,0,41,MPI_COMM_WORLD,&s);

      for (int i=1;i<numprocs;i++)
		{
			 MPI_Send(&RGBNewL,3,MPI_DOUBLE,i,42,MPI_COMM_WORLD);
	         MPI_Send(&RGBNewF,3,MPI_DOUBLE,i,43,MPI_COMM_WORLD);
		}	
	}

	else
	{
		MPI_Recv(meanFRGB,3,MPI_DOUBLE,0,42,MPI_COMM_WORLD,&s);
		MPI_Recv(meanLRGB,3,MPI_DOUBLE,0,43,MPI_COMM_WORLD,&s);
	}

double *fenZiMuUpLowRGB;

ComputeCorrelationXS(colorDataF,colorDataL,meanFRGB,meanLRGB,fenZiMuUpLowRGB, length);

	if (myid==0)
	{				
	    double *fenZiMuRGBTotal;   
        fenZiMuRGBTotal=(double*)malloc(sizeof(double)*9*numprocs);	
        MPI_Send(fenZiMuUpLowRGB,9,MPI_DOUBLE,0,50,MPI_COMM_WORLD);
		MPI_Recv(fenZiMuRGBTotal,9,MPI_DOUBLE,0,50,MPI_COMM_WORLD,&s);

       		
		for (int i=1;i<numprocs;i++)
		{
		//	MPI_Status s;
		
			MPI_Recv(fenZiMuRGBTotal+9*i,9,MPI_DOUBLE,i,51,MPI_COMM_WORLD,&s);
		}
        
         double fenZiMuRGBCal[9];
			for (int i=0;i<9;i++)
				fenZiMuRGBCal[i]=0;

           
			for (int i=0;i<numprocs*9;)			
			{
		     	for (int k=0;k<9;k++)
					fenZiMuRGBCal[k]=fenZiMuRGBCal[k]+fenZiMuRGBTotal[i++];
			}

   double CorrelationXS[3];
   CorrelationXS[0]=fenZiMuRGBCal[0]/(sqrt(fenZiMuRGBCal[1]*fenZiMuRGBCal[2]));
   CorrelationXS[1]=fenZiMuRGBCal[3]/(sqrt(fenZiMuRGBCal[4]*fenZiMuRGBCal[5]));
   CorrelationXS[2]=fenZiMuRGBCal[6]/(sqrt(fenZiMuRGBCal[7]*fenZiMuRGBCal[8]));

   printf("CorrelationXS[0]=%f,CorrelationXS[1]=%f,CorrelationXS[2]=%f\n\n",CorrelationXS[0],CorrelationXS[1],CorrelationXS[2]);
   
	} 

	else
	{ 	
	MPI_Send(fenZiMuUpLowRGB,9,MPI_DOUBLE,0,51,MPI_COMM_WORLD);
	}
	
    MPI_Finalize(); 

/*		
	endtime = MPI_Wtime();
    printf("That takes %f seconds:\n",endtime-starttime);
    MPI_Finalize();  */

    return 0;
}
