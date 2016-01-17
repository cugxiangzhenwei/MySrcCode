// PCA.cpp : 定义控制台应用程序的入口点。
//
// PCA.cpp : Defines the entry point for the console application.
//
//#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <complex>
#include "mpi.h"

//double *dMulspecMeanGray;
//dMulspecMeanGray=(double*)malloc(sizeof(double)*3);

bool ReadColorBmp(char *name, unsigned char *&body, unsigned char *&head, int &length)
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

 

//bool ComputedCovariance(unsigned char *&pColor, double *&dConariance, int length)
bool ComputedCovariance(unsigned char *&pColor, double *&dConariance, double *dMulspecMeanGray,int length)
//bool ComputedCovariance(unsigned char *&pColor, double *&dConariance, double x, double y,double z,int length)
{
 // dConariance=(double*)malloc(sizeof(double)*9);
   int i;
  // 	dConariance=(double*)malloc(sizeof(double)*9);

//	double *dMulspecMeanGray;
//dMulspecMeanGray=(double*)malloc(sizeof(double)*3);

	//    dMulspecMeanGray[0] = x;
	//	dMulspecMeanGray[1] = y;
     //    dMulspecMeanGray[2] =z;

   /* double *dMulspecMeanGray,*temp;

	temp=(double*)malloc(sizeof(double)*3);
	dMulspecMeanGray=(double*)malloc(sizeof(double)*3);
	dConariance=(double*)malloc(sizeof(double)*9);

    for(int i=0;i<3;i++)
	{
	    dMulspecMeanGray[i] = 0;
		temp[i] = 0;
	}

	for(i=0;i<length*3;)
	{	    
		double B=pColor[i++];
		double G=pColor[i++];
		double R=pColor[i++];

		temp[0]+=B;	//sum B
		temp[1]+=G;	//sum G
		temp[2]+=R;	//sum R
	}

	for( i=0;i<9;i++)
	{
		dConariance[i]=0;
	}

	dMulspecMeanGray[0]=temp[0]/length;	//average B
	dMulspecMeanGray[1]=temp[1]/length;	//average G
	dMulspecMeanGray[2]=temp[2]/length;	//average R */

   


    for(i=0;i<length;i++)
	{
	    dConariance[0]+=((pColor[3*i]-dMulspecMeanGray[0])*(pColor[3*i]-dMulspecMeanGray[0]));
	}

	for(i=0;i<length;i++)
	{
	    dConariance[1]+=((pColor[3*i]-dMulspecMeanGray[0])*(pColor[1+3*i]-dMulspecMeanGray[1]));
	}

	for(i=0;i<length;i++)
	{
	    dConariance[2]+=((pColor[3*i]-dMulspecMeanGray[0])*(pColor[2+3*i]-dMulspecMeanGray[2]));
	}

	for(i=0;i<length;i++)
	{
	    dConariance[3]+=((pColor[3*i]-dMulspecMeanGray[0])*(pColor[1+3*i]-dMulspecMeanGray[1]));
	}

	for(i=0;i<length;i++)
	{
	    dConariance[4]+=((pColor[1+3*i]-dMulspecMeanGray[1])*(pColor[1+3*i]-dMulspecMeanGray[1]));
	}

	for(i=0;i<length;i++)
	{
	    dConariance[5]+=((pColor[1+3*i]-dMulspecMeanGray[1])*(pColor[2+3*i]-dMulspecMeanGray[2]));
	}

	for(i=0;i<length;i++)
	{
		dConariance[6]+=((pColor[3*i]-dMulspecMeanGray[0])*(pColor[2+3*i]-dMulspecMeanGray[2]));
	}

	for(i=0;i<length;i++)
	{
	    dConariance[7]+=((pColor[1+3*i]-dMulspecMeanGray[1])*(pColor[2+3*i]-dMulspecMeanGray[2]));
	}

	for(i=0;i<length;i++)
	{
	    dConariance[8]+=((pColor[2+3*i]-dMulspecMeanGray[2])*(pColor[2+3*i]-dMulspecMeanGray[2]));
	}



/*	for(i=0;i<9;i++)
	{
	    dConariance[i]/=length;
	} */
	return true;
}



///////////////////////////////////////////////////////
//求矩阵特征值和特征向量
//参数说明：
//a--双精度实型二维数组指针，存放实对称系数矩阵A；
//n--矩阵的阶数；
//v--双精度实型二维数组指针，存放特征向量
//eps--控制精度要求。
void JacobiCharacterVector(double *&a, int n, double *&v, double eps)
{

	int i,j,p,q,u,w,t,s;
    double ff,fm,cn,sn,omega,x,y,d;
	v=(double*)malloc(sizeof(double)*9);

    for (i=0; i<=n-1; i++)
	{ 
		v[i*n+i]=1.0;
     	for (j=0; j<=n-1; j++)
			if (i!=j)
				v[i*n+j]=0.0;
	}
    ff=0.0;
    for (i=1; i<=n-1; i++)
		for (j=0; j<=i-1; j++)
		{ 
			d=a[i*n+j]; ff=ff+d*d; 
		}
	ff=sqrt(2.0*ff);
loop0:
	ff=ff/(1.0*n);
loop1:
    for (i=1; i<=n-1; i++)
	    for (j=0; j<=i-1; j++)
		{ 
			d=fabs(a[i*n+j]);
            if (d>ff)
			{ 
				p=i; 
				q=j;
		    	goto loop;
			}
		}
		if (ff<eps) return;
		goto loop0;
loop:
	u=p*n+q; 
   	w=p*n+p; 
   	t=q*n+p; 
	s=q*n+q;
    x=-a[u];
	y=(a[s]-a[w])/2.0;
	omega=x/sqrt(x*x+y*y);
	if (y<0.0) omega=-omega;
    sn=1.0+sqrt(1.0-omega*omega);
	sn=omega/sqrt(2.0*sn);
	cn=sqrt(1.0-sn*sn);
	fm=a[w];
	a[w]=fm*cn*cn+a[s]*sn*sn+a[u]*omega;
	a[s]=fm*sn*sn+a[s]*cn*cn-a[u]*omega;
	a[u]=0.0; a[t]=0.0;
	for (j=0; j<=n-1; j++)
		if ((j!=p)&&(j!=q))
        {
			u=p*n+j;
			w=q*n+j;
		    fm=a[u];
		    a[u]=fm*cn+a[w]*sn;
		    a[w]=-fm*sn+a[w]*cn;
          }
	 for (i=0; i<=n-1; i++)
		  if ((i!=p)&&(i!=q))
		  {
			  u=i*n+p; 
			  w=i*n+q;
              fm=a[u];
              a[u]=fm*cn+a[w]*sn;
              a[w]=-fm*sn+a[w]*cn;
		  }
	  for (i=0; i<=n-1; i++)
	  {
		  u=i*n+p;
		  w=i*n+q;
		  fm=v[u];
		  v[u]=fm*cn+v[w]*sn;
		  v[w]=-fm*sn+v[w]*cn;
	  }
	  goto loop1;
	//按特征值由大到小的顺序排列特征向量。
/*	for(i=0;i<2;i++)
	{
		for(j=i+1;j<3;j++)
		{
			if(a[j*3+j] > a[i*3+i])
			{
				double temp = 0;
				temp = a[j*3+j];
				a[j*3+j] = a[i*3+i];
				a[i*3+i] = temp;
				for(int k=0;k<3;k++)
				{
					temp = 0;
					temp = v[k*3+j];
					v[k*3+j] = v[k*3+i];
					v[k*3+i] = temp;
				}
			}
		}			
	}
  */
	delete []a;
}


void TransMatrix(double *a, int m, int n)
{
	double*p;
	if((p=new double[m*n])==NULL)
		 return;
	double temp =0;
	int i=0;

	for(i=0;i<m;i++)
		for(int j=0;j<n;j++)
		{			
			*(p+i*n+j) = *(a+j*m+i);
		}
	for( i=0;i<m;i++)
		for(int j=0;j<n;j++)
		{			
			*(a+i*n+j) = *(p+i*n+j);
		}
	delete []p;
}



//bool ComputeP(unsigned char *&P, double *&CharacterVector, unsigned char *&pColor, int length)
bool ComputeP(double *&P, double *&CharacterVector, unsigned char *&pColor, int length)
{
	//P=(unsigned char*)malloc(sizeof(double)*length*3);
      
       P=(double*)malloc(sizeof(double)*length*3);
	
   /* for(int i=0,k=0,l=0;i<=6;i+=3)
    {
        for(int j=0;j<=(length-1)*3;j+=3)
        {
            P[k]=(unsigned char)(CharacterVector[i]*pColor[j]+CharacterVector[i+1]*pColor[j+1]+CharacterVector[i+2]*pColor[j+2]);
            k+=3;
        }
        l++;
        k=l;
    } */

	  int i=0;
     for( i=0;i<3*length;i++)
          P[i]=0;

  
 for(i=0;i<length;i++)
	  {    
          P[i*3]=(CharacterVector[0]*pColor[i*3]+CharacterVector[1]*pColor[i*3+1]+CharacterVector[2]*pColor[i*3+2]);              
          P[i*3+1]=(CharacterVector[3]*pColor[i*3]+CharacterVector[4]*pColor[i*3+1]+CharacterVector[5]*pColor[i*3+2]); 
          P[i*3+2]=(CharacterVector[6]*pColor[i*3]+CharacterVector[7]*pColor[i*3+1]+CharacterVector[8]*pColor[i*3+2]); 
      }



  /*  for(int j=0;j<=6;j+=3)
    {
        for(int k=0;k<3;k++)
        {
            P[i*3]=(unsigned char)(CharacterVector[i]*pColor[j]+CharacterVector[i+1]*pColor[j+1]+CharacterVector[i+2]*pColor[j+2]);
            k+=3;
        }
        l++;
        k=l;
    }
  */

	return true;
}

//bool ComputePnew(unsigned char *&P, unsigned char *&pGray, int length)
bool ComputePnew(double *&P, unsigned char *&pGray, int length)
{
    //for(int i=0,j=0;i<length;j=i*3)
        for(int i=0;i<length;i++)
        P[i*3]=pGray[i];
    	return true;
}

bool ComputeNewColor(double *&pColor, double*&P, double *&CharacterVector, int length)
//bool ComputeNewColor(unsigned char*&pColor, double *&P, double *&CharacterVector, int length)
{
  
    
 /*   for(int i=0,k=0,l=0;i<=6;i+=3)
    {
        for(int j=0;j<=(length-1)*3;j+=3)
        {

            pColor[k]=(unsigned char)(CharacterVector[i]*P[j]+CharacterVector[i+1]*P[j+1]+CharacterVector[i+2]*P[j+2]);
            k+=3;
        }
        l++;
        k=l;
    } */

pColor=(double*)malloc(sizeof(double)*length*3);



for(int i=0;i<length;i++)
	  {    
          pColor[i*3]=(CharacterVector[0]*P[i*3]+CharacterVector[1]*P[i*3+1]+CharacterVector[2]*P[i*3+2]);              
          pColor[i*3+1]=(CharacterVector[3]*P[i*3]+CharacterVector[4]*P[i*3+1]+CharacterVector[5]*P[i*3+2]); 
          pColor[i*3+2]=(CharacterVector[6]*P[i*3]+CharacterVector[7]*P[i*3+1]+CharacterVector[8]*P[i*3+2]); 
      }

 /*   for(i=0;i<length*3;i++)
    {
        if(pColor[i]>=255)
            pColor[i]=255;
        if(pColor[i]<=0)
            pColor[i]=0;        
    } */
	return true;        
}

bool AddColor(unsigned char *&pColor, double*&RGB, int length)
{   
	RGB=(double*)malloc(sizeof(double)*3);

	int i=0;

    for(i=0;i<3;i++)
	{	   
		RGB[i] = 0;
	}

	for(i=0;i<length*3;)
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

int main(int argc, char *argv[])
{
	
    int myid, numprocs;
//	double starttime, endtime;
	MPI_Status s;
	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);	
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);
    int i;
    unsigned char *colorData;
//	unsigned char* grayData;
	unsigned char* colorHead;
	int length=0;

	double starttime,endtime;
	starttime = MPI_Wtime();

    double *RGB;      
    double  *colorBodyNew;	
//	int colorLength, grayLength;
	int colorLength;
//	double eps = 0.000001;   //控制精度要求

 RGB=(double*)malloc(sizeof(double)*3);

  /*   for(int i=0;i<3;i++)
	{
	   
		RGB[i] = 0;
	} */ 
 double RGBNew[3];

		for(i=0;i<3;i++)
		{	   
		  RGBNew[i]=0;
         // temp[i] =0;
		} 

 fprintf(stdout,"Process %d of %d on %s\n",myid,numprocs,processor_name);

///////////////////////////////////////////////////////////////////////////

if (myid==0)
	{
	
       	unsigned char* colorBody;	
		
	    ReadColorBmp("F:\\代码备份\\MPIFusion\\TestData\\Mul.bmp", colorBody, colorHead, colorLength);

		printf("colorLength=%d\n",colorLength);        

		while ((colorLength%numprocs)!=0)
		{
			numprocs--;
		}
		length=colorLength/numprocs;
		colorData=colorBody;
		  
	    AddColor(colorData, RGB, colorLength); 
	    printf("RGB[0]=%f\n",RGB[0]/colorLength);
        printf("RGB[1]=%f\n",RGB[1]/colorLength);
        printf("RGB[2]=%f\n",RGB[2]/colorLength);
		
		for (int i=1;i<numprocs;i++)
		{
			MPI_Send(&length,1,MPI_INT,i,0,MPI_COMM_WORLD);
			MPI_Send(colorBody+i*length*3,length*3,MPI_UNSIGNED_CHAR,i,1,MPI_COMM_WORLD);
		//	MPI_Send(grayBody+i*length,length,MPI_UNSIGNED_CHAR,i,2,MPI_COMM_WORLD);
		}	
		
	}

	else
	{
	//	MPI_Status s;
		MPI_Recv(&length,1,MPI_INT,0,0,MPI_COMM_WORLD,&s);
		colorData=(unsigned char* )malloc(sizeof(unsigned char)*length*3);
	//	grayData=(unsigned char* )malloc(sizeof(unsigned char)*length);
		MPI_Recv(colorData,length*3,MPI_UNSIGNED_CHAR,0,1,MPI_COMM_WORLD,&s);
	//	MPI_Recv(grayData,length,MPI_UNSIGNED_CHAR,0,2,MPI_COMM_WORLD,&s);
	}
        	
    //printf("colorLength=%d\n",colorLength);
	  printf("length=%d\n",length);     
	  AddColor(colorData, RGB, length);  

	if (myid==0)
	{				
	    double *RGBData;   
        RGBData=(double*)malloc(sizeof(double)*3*numprocs);	
        MPI_Send(RGB,3,MPI_DOUBLE,0,20,MPI_COMM_WORLD);
		MPI_Recv(RGBData,3,MPI_DOUBLE,0,20,MPI_COMM_WORLD,&s);
		
		for (int i=1;i<numprocs;i++)
		{
		//	MPI_Status s;
			//MPI_Recv(&RGBData+3*i,3,MPI_DOUBLE,i,2,MPI_COMM_WORLD,&s);
			MPI_Recv(RGBData+3*i,3,MPI_DOUBLE,i,200,MPI_COMM_WORLD,&s);
		}


		double temp0,temp1,temp2;
		temp0=0;
	    temp1=0;
	    temp2=0;

	/*	double RGBNew[3];

		for(i=0;i<3;i++)
		{	   
		  RGBNew[i]=0;
         // temp[i] =0;
		}    */    
     
        for (i=0;i<numprocs*3;)
		{
         
		 temp0=RGBData[i++];           
         temp1=RGBData[i++];
         temp2=RGBData[i++];
           
	     RGBNew[0]+=temp0;
		 RGBNew[1]+=temp1;
		 RGBNew[2]+=temp2;	  

		}

	  printf("\n");           
      printf("colorLength=%d\n",colorLength);   	  
      RGBNew[0]=RGBNew[0]/colorLength;
      RGBNew[1]=RGBNew[1]/colorLength;
      RGBNew[2]=RGBNew[2]/colorLength; 
	  
      printf("RGBNew[0]=%f\n",RGBNew[0]);
      printf("RGBNew[1]=%f\n",RGBNew[1]);
      printf("RGBNew[2]=%f\n",RGBNew[2]);


    printf("length[]=%d\n",length);
	unsigned char* colorBodyNew;
	unsigned char* pColor;

	
/*	ReadColorBmp("C:\\XXR_Image\\tm.bmp", colorBodyNew, colorHead, colorLength);

	pColor=colorBodyNew;

//	ComputedCovariance(colorDataNew, dConariance2,RGBNew[0],RGBNew[1],RGBNew[2],colorLength);
///////////////////////////////////////////////////////////////////////////////////////////////////////

 //int i;
    	double *dConariance;

   	dConariance=(double*)malloc(sizeof(double)*9);
 for(i=0;i<9;i++)
	{
	    dConariance[i]=0;
	}


	double *dMulspecMeanGray;
dMulspecMeanGray=(double*)malloc(sizeof(double)*3);

	    dMulspecMeanGray[0] =RGBNew[0];
		dMulspecMeanGray[1] = RGBNew[1];
         dMulspecMeanGray[2] =RGBNew[2];

   /* double *dMulspecMeanGray,*temp;

	temp=(double*)malloc(sizeof(double)*3);
	dMulspecMeanGray=(double*)malloc(sizeof(double)*3);
	dConariance=(double*)malloc(sizeof(double)*9);

    for(int i=0;i<3;i++)
	{
	    dMulspecMeanGray[i] = 0;
		temp[i] = 0;
	}

	for(i=0;i<length*3;)
	{	    
		double B=pColor[i++];
		double G=pColor[i++];
		double R=pColor[i++];

		temp[0]+=B;	//sum B
		temp[1]+=G;	//sum G
		temp[2]+=R;	//sum R
	}

	for( i=0;i<9;i++)
	{
		dConariance[i]=0;
	}

	dMulspecMeanGray[0]=temp[0]/length;	//average B
	dMulspecMeanGray[1]=temp[1]/length;	//average G
	dMulspecMeanGray[2]=temp[2]/length;	//average R */ 

  /* 


    ///////////////////////////
printf("协方差矩阵\n");
     for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			printf("%f\n",dConariance[i*3+j]);
         	printf("\n");
	}
    printf("\n\n");*/
/////////////////////////////


	} 

	else
	{ 	

		MPI_Send(RGB,3,MPI_DOUBLE,0,200,MPI_COMM_WORLD);
	}

  ////////////协方差矩阵计算////////////////////////////
    double *dMulspecMeanGray2;
	dMulspecMeanGray2=(double*)malloc(sizeof(double)*3);

   if (myid==0)
	{				
	    MPI_Send(&RGBNew,3,MPI_DOUBLE,0,401,MPI_COMM_WORLD);
        MPI_Recv(dMulspecMeanGray2,3,MPI_DOUBLE,0,401,MPI_COMM_WORLD,&s);

		for (int i=1;i<numprocs;i++)
		{
		   MPI_Send(&RGBNew,3,MPI_DOUBLE,i,400,MPI_COMM_WORLD);
		}
   }
    else
	{ 	

		MPI_Recv(dMulspecMeanGray2,3,MPI_DOUBLE,0,400,MPI_COMM_WORLD,&s);
	}


	double *dConariance2;
	dConariance2=(double*)malloc(sizeof(double)*9);

		for( i=0;i<9;i++)
	{
		dConariance2[i]=0;
	}

   
   for (i=0;i<3;i++)
		{
	//	dMulspecMeanGray2[i]=RGBNew[i];
         printf("dMulspecMeanGray2[]=%f\n",dMulspecMeanGray2[i]); 
		}/* */
    printf("length[]=%d\n\n",length); 

	ComputedCovariance(colorData, dConariance2,dMulspecMeanGray2,length);

	/* */

 
if (myid==0)
	{	
	
	    double *TConariance;   
        TConariance=(double*)malloc(sizeof(double)*9*numprocs);	
	    for(i=0;i<9*numprocs;i++)
		{	   
		TConariance[i] = 0;
		}
	   
        MPI_Send(dConariance2,9,MPI_DOUBLE,0,300,MPI_COMM_WORLD);
		MPI_Recv(TConariance,9,MPI_DOUBLE,0,300,MPI_COMM_WORLD,&s);
		
		for (int i=1;i<numprocs;i++)
		{
		//	MPI_Status s;
			//MPI_Recv(&RGBData+3*i,3,MPI_DOUBLE,i,2,MPI_COMM_WORLD,&s);
			MPI_Recv(TConariance+9*i,9,MPI_DOUBLE,i,301,MPI_COMM_WORLD,&s);
		}
        
        double temp[9];		

		double TConarNew[9];

		for(i=0;i<9;i++)
		{	   
		  TConarNew[i]=0;
          temp[i] =0;
		}     
     
        for (i=0;i<numprocs*9;)
		{
         
		 temp[0]=TConariance[i++];           
         temp[1]=TConariance[i++];
         temp[2]=TConariance[i++];

		 temp[3]=TConariance[i++];           
         temp[4]=TConariance[i++];
         temp[5]=TConariance[i++];

		 temp[6]=TConariance[i++];           
         temp[7]=TConariance[i++];
         temp[8]=TConariance[i++];
           
	     TConarNew[0]+=temp[0];
		 TConarNew[1]+=temp[1];
		 TConarNew[2]+=temp[2];

		 
		 TConarNew[3]+=temp[3];
		 TConarNew[4]+=temp[4];
		 TConarNew[5]+=temp[5];

		 TConarNew[6]+=temp[6];
		 TConarNew[7]+=temp[7];
		 TConarNew[8]+=temp[8];	  

		}

		printf("\n\n协方差矩阵%d\n",colorLength);
        for(i=0;i<3;i++)
		  for(int j=0;j<3;j++)
			TConarNew[i*3+j]=TConarNew[i*3+j]/colorLength;
        


        for(i=0;i<3;i++)
		{
		for(int j=0;j<3;j++)
			printf("%f  ",TConarNew[i*3+j]);
         	printf("\n\n");
		}
        printf("\n\n");
//////////////////////////////////////////////
	double* mainP;
	double* Conariance;
	double* mainVector;
   // double temp;    
   
   // double* colorBody;
	
	int colorLength, grayLength;
	double eps = 0.000001;   //控制精度要求

	double *NewConariance;
	NewConariance=(double*)malloc(sizeof(double)*9);

	for(i=0;i<9;i++)
		NewConariance[i]=TConarNew[i];
	

	JacobiCharacterVector(NewConariance, 3, mainVector, eps);



/////////////////////////////　
/*printf("特征值矩阵\n");
    for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			printf("%f   ",Conariance[i*3+j]);
         	printf("\n");
	}*/
    printf("\n\n"); 
/////////////////////////////   



/////////////////////////////
   printf("原始的Vector\n");

    for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			printf("%f   ",mainVector[i*3+j]);
         	printf("\n");
	}
    printf("\n\n");
/////////////////////////////

   
  

   	//按特征值由大到小的顺序排列特征向量。
//	for(int i=0;i<2;i++)
    for(i=0;i<2;i++)
	{
		for(int j=i+1;j<3;j++)
		{
			if(NewConariance[j*3+j] > NewConariance[i*3+i])
			{
				double temp = 0;
				temp = NewConariance[j*3+j];
				NewConariance[j*3+j] = NewConariance[i*3+i];
			NewConariance[i*3+i] = temp;
				for(int k=0;k<3;k++)
				{
					temp = 0;
					temp = mainVector[k*3+j];
					mainVector[k*3+j] = mainVector[k*3+i];
					mainVector[k*3+i] = temp;
				}
			}
		}			
	}



/////////////////////////////
 printf("排列的Vector\n"); 

   for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			printf("%f   ",mainVector[i*3+j]);
         	printf("\n");
	}
   printf("\n\n");
/////////////////////////////    





TransMatrix(mainVector, 3, 3);


/////////////////////////////
 printf("转置的Vector\n"); 
for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			printf("%f   ",mainVector[i*3+j]);
         	printf("\n");
	}
    printf("\n\n");
//printf("colorLength=%d\n\n\n",colorLength);
/////////////////////////////
	unsigned char* colorBody;
    unsigned char* grayBody;
    // int grayLength;

	 ReadColorBmp("F:\\代码备份\\MPIFusion\\TestData\\Mul.bmp", colorBody, colorHead, colorLength);

    ReadGrayBmp("F:\\代码备份\\MPIFusion\\TestData\\Pan.bmp", grayBody, grayLength);

	ComputeP(mainP, mainVector, colorBody, colorLength);
	ComputePnew(mainP, grayBody, colorLength);


/////////////////////////////
	printf("替换后的最后一个像素的三个值＝%f, %f,%f\n\n",mainP[719997],mainP[719998],mainP[719999]);
/////////////////////////////

    //线性改正，将像素值分波段改到正数的范围内
	double dMax[3],dMin[3];
	
	for(i=0;i<3;i++)
	{
		dMax[i] = -255;
		dMin[i] = 255;
	}
     
//统计极值
	for(i=0;i<colorLength;i++)
		for(int k=0;k<3;k++)
		{
		
				if(dMax[k]<mainP[i*3+k])
					dMax[k] = mainP[i*3+k];
				if(dMin[k]>mainP[i*3+k])
					dMin[k] =mainP[i*3+k];
		}
		
	
	//线性拉伸
for(i=0;i<colorLength;i++)
		for(int k=0;k<3;k++)
			{				
				if((dMax[k]-dMin[k])<255)
					mainP[i*3+k]= (mainP[i*3+k]-dMin[k]);
				else
						mainP[i*3+k] = (mainP[i*3+k]-dMin[k])*255/(dMax[k]-dMin[k]);
			}			
	





  TransMatrix(mainVector, 3, 3);

	ComputeNewColor(colorBodyNew, mainP, mainVector, colorLength);

/////////////////////////////
	printf("逆变换后的最后一个像素的三个值＝%f, %f,%f\n",colorBodyNew[719997],colorBodyNew[719998],colorBodyNew[719999]);
/////////////////////////////


//统计极值
	for(i=0;i<colorLength;i++)
		for(int k=0;k<3;k++)
			{
			
				if(dMax[k]<colorBodyNew[i*3+k])
					dMax[k] = colorBodyNew[i*3+k];
				if(dMin[k]>colorBodyNew[i*3+k])
					dMin[k] =colorBodyNew[i*3+k];
			}


/////////////////////////////
printf("\n\n"); 
printf("dMax=");
int k=0;
for( k=0;k<3;k++)
			{
			
			printf("%f 	",dMax[k]); 
			
			}
	printf("\n"); 
printf("dMin=");
for(k=0;k<3;k++)
			{
			
			printf("%f 	",dMin[k]); 
			
			}
	printf("\n\n"); 
/////////////////////////////
	

	//线性拉伸
	for(i=0;i<colorLength;i++)
		for(int k=0;k<3;k++)
			{
				if((dMax[k]-dMin[k])<255)
					colorBodyNew[i*3+k]= (colorBodyNew[i*3+k]-dMin[k]);
				else
						colorBodyNew[i*3+k] = (colorBodyNew[i*3+k]-dMin[k])*255/(dMax[k]-dMin[k]);
			}	
		
/////////////////////////////
printf("线性拉伸后的最后一个像素的三个值＝%f, %f,%f\n\n",colorBodyNew[719997],colorBodyNew[719998],colorBodyNew[719999]);
/////////////////////////////


for(i=0;i<colorLength;i++)
		for(int k=0;k<3;k++)
			colorBody[i*3+k]=(unsigned char)(colorBodyNew[i*3+k]); 

/////////////////////////////
printf("变为unsigned char后的最后一个像素的三个值＝%d, %d,%d\n\n",colorBody[719997],colorBody[719998],colorBody[719999]);
/////////////////////////////

	WriteColorBmp("F:\\代码备份\\MPIFusion\\TestData\\PCA_Color.bmp", colorBody, colorHead, colorLength);

		delete []colorBodyNew;
        	delete []colorBody;







	} 

	else
	{ 	
        
		MPI_Send(dConariance2,9,MPI_DOUBLE,0,301,MPI_COMM_WORLD);
	
	}
/////////////////////////////////////////////////////////


   
/////////////////////////////
   

	//JacobiCharacterVector(Conariance, 3, mainVector, eps);





 /*  */
	
    MPI_Finalize(); 
	
    return 0; 
}







/*


int main(int argc, char *argv[])
{
	
	unsigned char* colorHead;
	unsigned char* colorBody;
	unsigned char* grayBody;
	
   // unsigned char* mainP;
    double* mainP;
	double* Conariance;
	double* mainVector;
    double temp;    
   
    double* colorBodyNew;
	
	int colorLength, grayLength;
	double eps = 0.000001;   //控制精度要求

	ReadColorBmp("C:\\XXR-C盘－综合\\tm.bmp", colorBody, colorHead, colorLength);
	ReadGrayBmp("C:\\XXR-C盘－综合\\spot.bmp", grayBody, grayLength);

	//Convert(colorBody, grayBody, colorLength);
	ComputedCovariance(colorBody, Conariance, colorLength);


///////////////////////////
printf("协方差矩阵\n");
     for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			printf("%f   ",Conariance[i*3+j]);
         	printf("\n");
	}
    printf("\n\n");
/////////////////////////////
   

	JacobiCharacterVector(Conariance, 3, mainVector, eps);



/////////////////////////////　
printf("特征值矩阵\n");
    for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			printf("%f   ",Conariance[i*3+j]);
         	printf("\n");
	}
    printf("\n\n"); 
/////////////////////////////   



/////////////////////////////
   printf("原始的Vector\n");

    for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			printf("%f   ",mainVector[i*3+j]);
         	printf("\n");
	}
    printf("\n\n");
/////////////////////////////

   
  

   	//按特征值由大到小的顺序排列特征向量。
//	for(int i=0;i<2;i++)
    for(i=0;i<2;i++)
	{
		for(int j=i+1;j<3;j++)
		{
			if(Conariance[j*3+j] > Conariance[i*3+i])
			{
				double temp = 0;
				temp = Conariance[j*3+j];
				Conariance[j*3+j] = Conariance[i*3+i];
				Conariance[i*3+i] = temp;
				for(int k=0;k<3;k++)
				{
					temp = 0;
					temp = mainVector[k*3+j];
					mainVector[k*3+j] = mainVector[k*3+i];
					mainVector[k*3+i] = temp;
				}
			}
		}			
	}



/////////////////////////////
 printf("排列的Vector\n"); 

   for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			printf("%f   ",mainVector[i*3+j]);
         	printf("\n");
	}
   printf("\n\n");
/////////////////////////////    





TransMatrix(mainVector, 3, 3);


/////////////////////////////
 printf("转置的Vector\n"); 
for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			printf("%f   ",mainVector[i*3+j]);
         	printf("\n");
	}
    printf("\n\n");
printf("colorLength=%d\n\n\n",colorLength);
/////////////////////////////
	

	ComputeP(mainP, mainVector, colorBody, colorLength);
	ComputePnew(mainP, grayBody, colorLength);


/////////////////////////////
	printf("替换后的最后一个像素的三个值＝%f, %f,%f\n\n",mainP[719997],mainP[719998],mainP[719999]);
/////////////////////////////

    //线性改正，将像素值分波段改到正数的范围内
	double dMax[3],dMin[3];
	
	for(i=0;i<3;i++)
	{
		dMax[i] = -255;
		dMin[i] = 255;
	}
     
//统计极值
	for(i=0;i<colorLength;i++)
		for(int k=0;k<3;k++)
		{
		
				if(dMax[k]<mainP[i*3+k])
					dMax[k] = mainP[i*3+k];
				if(dMin[k]>mainP[i*3+k])
					dMin[k] =mainP[i*3+k];
		}
		
	
	//线性拉伸
for(i=0;i<colorLength;i++)
		for(int k=0;k<3;k++)
			{				
				if((dMax[k]-dMin[k])<255)
					mainP[i*3+k]= (mainP[i*3+k]-dMin[k]);
				else
						mainP[i*3+k] = (mainP[i*3+k]-dMin[k])*255/(dMax[k]-dMin[k]);
			}			
	





  TransMatrix(mainVector, 3, 3);

	ComputeNewColor(colorBodyNew, mainP, mainVector, colorLength);

/////////////////////////////
	printf("逆变换后的最后一个像素的三个值＝%f, %f,%f\n",colorBodyNew[719997],colorBodyNew[719998],colorBodyNew[719999]);
/////////////////////////////


//统计极值
	for(i=0;i<colorLength;i++)
		for(int k=0;k<3;k++)
			{
			
				if(dMax[k]<colorBodyNew[i*3+k])
					dMax[k] = colorBodyNew[i*3+k];
				if(dMin[k]>colorBodyNew[i*3+k])
					dMin[k] =colorBodyNew[i*3+k];
			}


/////////////////////////////
printf("\n\n"); 
printf("dMax=");
for( int k=0;k<3;k++)
			{
			
			printf("%f 	",dMax[k]); 
			
			}
	printf("\n"); 
printf("dMin=");
for(k=0;k<3;k++)
			{
			
			printf("%f 	",dMin[k]); 
			
			}
	printf("\n\n"); 
/////////////////////////////
	

	//线性拉伸
	for(i=0;i<colorLength;i++)
		for(int k=0;k<3;k++)
			{
				if((dMax[k]-dMin[k])<255)
					colorBodyNew[i*3+k]= (colorBodyNew[i*3+k]-dMin[k]);
				else
						colorBodyNew[i*3+k] = (colorBodyNew[i*3+k]-dMin[k])*255/(dMax[k]-dMin[k]);
			}	
		
/////////////////////////////
printf("线性拉伸后的最后一个像素的三个值＝%f, %f,%f\n\n",colorBodyNew[719997],colorBodyNew[719998],colorBodyNew[719999]);
/////////////////////////////


for(i=0;i<colorLength;i++)
		for(int k=0;k<3;k++)
			colorBody[i*3+k]=(unsigned char)(colorBodyNew[i*3+k]); 

/////////////////////////////
printf("变为unsigned char后的最后一个像素的三个值＝%d, %d,%d\n\n",colorBody[719997],colorBody[719998],colorBody[719999]);
/////////////////////////////

	WriteColorBmp("c:\\color.bmp", colorBody, colorHead, colorLength);

		delete []colorBodyNew;
        	delete []colorBody;

	return 0;
} */
