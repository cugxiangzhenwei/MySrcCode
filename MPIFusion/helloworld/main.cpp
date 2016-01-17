#include"mpi.h"
#include <stdio.h>
#include <string.h>
void  main(int  argc,  char **argv)
{
	int  numprocs, myrank, i, j, k;
	MPI_Status status;
	char msg[20]; 
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

	if(myrank == 0)
	{
		strcpy(msg,"Hello World");
		MPI_Send(msg,strlen(msg) + 1,MPI_CHAR, 1,99,MPI_COMM_WORLD);
	}
	else if( myrank == 1)
	{
		MPI_Recv(msg,20,MPI_CHAR,0,99, MPI_COMM_WORLD,&status);
		printf("Receive message = %s\n", msg);
	}
	MPI_Finalize();
}
