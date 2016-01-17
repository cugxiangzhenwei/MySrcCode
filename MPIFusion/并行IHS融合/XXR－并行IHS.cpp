#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

bool Convert(unsigned char *&pColor, unsigned char *&pGray, int length)
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
			I=pGray[position];
			
			B=(1.0-S)*I;
			G=3*H*I+(1.0-3*H)*B;
			R=3*I-B-G;
		}

		else if(min==R)
		{
			H=(B-R)/(3*(I-R));
			S=1.0-(R/I);
			I=pGray[position];
			
			R=(1.0-S)*I;
			B=3*H*I+(1.0-3*H)*R;
			G=3*I-R-B;
		}
		
		else{
			H=(R-G)/(3*(I-G));
			S=1.0-(G/I);
			I=pGray[position];
			
			G=(1.0-S)*I;
			R=3*H*I+(1.0-3*H)*G;
			B=3*I-R-G;			
		}

		if (B>=255)
		{
			B=255;
		}
		if (B<=0)
		{
			B=0;
		}
		if (G>=255)
		{
			G=255;
		}
		if (G<=0)
		{
			G=0;
		}
		if (R>=255)
		{
			R=255;
		}
		if (R<=0)
		{
			R=0;
		}
		pColor[i-3]=(unsigned char)B;
		pColor[i-2]=(unsigned char)G;
		pColor[i-1]=(unsigned char)R;
	}
	return true;
}

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

int main(int argc, char *argv[])
{
	int myid, numprocs;
	double starttime, endtime;
	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);	
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);

	unsigned char* colorData;
	unsigned char* grayData;
	unsigned char* colorHead;
	int length=0;

	starttime = MPI_Wtime();

	if (myid==0)
	{
		unsigned char* colorBody;
		unsigned char* grayBody;
		
		int colorLength, grayLength;		
		ReadColorBmp("F:\\代码备份\\MPIFusion\\TestData\\Mul.bmp", colorBody, colorHead, colorLength);
		ReadGrayBmp("F:\\代码备份\\MPIFusion\\TestData\\Pan.bmp", grayBody, grayLength);
		while ((colorLength%numprocs)!=0)
		{
			numprocs--;
		}
		length=colorLength/numprocs;
		colorData=colorBody;
		grayData=grayBody;
		for (int i=1;i<numprocs;i++)
		{
			MPI_Send(&length,1,MPI_INT,i,0,MPI_COMM_WORLD);
			MPI_Send(colorBody+i*length*3,length*3,MPI_UNSIGNED_CHAR,i,1,MPI_COMM_WORLD);
			MPI_Send(grayBody+i*length,length,MPI_UNSIGNED_CHAR,i,2,MPI_COMM_WORLD);
		}
		
	}
	else
	{
		MPI_Status s;
		MPI_Recv(&length,1,MPI_INT,0,0,MPI_COMM_WORLD,&s);
		colorData=(unsigned char* )malloc(sizeof(unsigned char)*length*3);
		grayData=(unsigned char* )malloc(sizeof(unsigned char)*length);
		MPI_Recv(colorData,length*3,MPI_UNSIGNED_CHAR,0,1,MPI_COMM_WORLD,&s);
		MPI_Recv(grayData,length,MPI_UNSIGNED_CHAR,0,2,MPI_COMM_WORLD,&s);
	}
	


	Convert(colorData, grayData, length);

	if (myid==0)
	{
		for (int i=1;i<numprocs;i++)
		{
			MPI_Status s;
			MPI_Recv(colorData+length*3*i,length*3,MPI_INT,i,3,MPI_COMM_WORLD,&s);
			
		}
		WriteColorBmp("F:\\代码备份\\MPIFusion\\TestData\\IHS_Color.bmp", colorData, colorHead, length*numprocs);
	} 
	else
	{
		MPI_Send(colorData,length*3,MPI_UNSIGNED_CHAR,0,3,MPI_COMM_WORLD);
	}

	endtime = MPI_Wtime();
	printf("That tooks %f secodes\n", endtime-starttime);

    MPI_Finalize();
	return 0;
}