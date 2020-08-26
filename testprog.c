/*
| Author: Julian Pineiro
| Source: testprog.c
|
*/

#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_LOC 1

void broadcast_p0_pow2();
void broadcast_pow2();
void broadcast();
int verify();

int main(int argc, char** argv) {

	int value = 0;
	char* rootText = "";
	

	MPI_Status status;

	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	int program = atoi(argv[1]);
	int val = atoi(argv[2]);
	int rootVal = atoi(argv[3]);
	
	int world_rank;

	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	if (world_rank == rootVal && program != 1)
	{
		value = val;
		rootText = "(root)";
	}
	else if(world_rank == 0 && program == 1)
	{
		value = val;
		rootText = "(root)";
	}
	else if(program == 4)
	{
		value = world_rank;

	}

	
	if(program == 1)
	{
	printf("Process %d (Initial):%d %s\n",world_rank,value,rootText);
	broadcast_p0_pow2(&value,MPI_COMM_WORLD);
	printf("Process %d:%d %s\n",world_rank,value,rootText);
	}

	if(program == 2)
	{
	printf("Process %d (Initial):%d %s\n",world_rank,value,rootText);
	broadcast_pow2(&value,rootVal,MPI_COMM_WORLD);
	printf("Process %d:%d %s\n",world_rank,value,rootText);
	}

	if (program == 3)
	{
	printf("Process %d (Initial):%d %s\n",world_rank,value,rootText);
	broadcast(&value,rootVal,MPI_COMM_WORLD);
	printf("Process %d:%d %s\n",world_rank,value,rootText);
	}
	

		
	MPI_Finalize();

} // end main



void broadcast_p0_pow2(int* x, MPI_Comm comm)
{
	// Get number of processes
	int world_size;
	MPI_Comm_size(comm, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(comm, &world_rank);

	MPI_Status status;

	double logVal = log2((double) world_size);
	double logValDif = floor(logVal);
	
	if(((logVal - logValDif) > 0) && (world_size > 0))
	{
		MPI_Abort(comm,1);
	}

	int dim = (int) logVal;
	int count = 0;
	int test = 1;
	
	for(count ; count < dim; count++)
	{
		if(world_rank < test)
		{
			MPI_Send(x, 1, MPI_INT, (test + world_rank), 0, comm);
		}
		else if(world_rank < (test * 2))
		{
			MPI_Recv(x, 1, MPI_INT, MPI_ANY_SOURCE, 0, comm, &status);
		}

		test = test * 2;
	}
	
}

void broadcast_pow2(int* x, int root, MPI_Comm comm)
{
	// Get number of processes
	int world_size;
	MPI_Comm_size(comm, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(comm, &world_rank);

	MPI_Status status;

	double logVal = log2((double) world_size);
	double logValDif = floor(logVal);
	
	if(((logVal - logValDif) > 0) && (world_size > 0))
	{
		MPI_Abort(comm,1);
	}

	int dim = (int) logVal;
	int offset = world_size - root;
	int sudoNode = ((world_rank + offset) % world_size);
	int sudoNodeRev = (sudoNode + root)%world_size;
	int count = 0;
	int test = 1;
	int newRank = 0;
	

	for(count ; count < dim; count++)
	{
		if(sudoNode < test)
		{
			int newSudoNode = test + sudoNode;
			MPI_Send(x, 1, MPI_INT,((newSudoNode + root)%world_size), 0, comm);
		}
		else if(sudoNode < (test * 2))
		{
			MPI_Recv(x, 1, MPI_INT, MPI_ANY_SOURCE, 0, comm, &status);
		}

		test = test * 2;
	}


}

void broadcast(int* x, int root, MPI_Comm comm)
{
	// Get number of processes
	int world_size;
	MPI_Comm_size(comm, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(comm, &world_rank);

	MPI_Status status;

	double logVal = log2((double) world_size);
	double logValDif = floor(logVal);

	int dim = (int) logValDif;

	if ((logVal - logValDif) > 0)
	{
		dim = dim + 1;
	}
	int offset = world_size - root;
	int sudoNode = ((world_rank + offset) % world_size);
	int sudoNodeRev = (sudoNode + root)%world_size;
	int count = 0;
	int test = 1;
	int newRank = 0;
	

	for(count ; count < dim; count++)
	{
		if(sudoNode < test)
		{
			int newSudoNode = test + sudoNode;
			int sendNode = (newSudoNode + root) % world_size;
			if(sendNode < world_size)
			{
			MPI_Send(x, 1, MPI_INT,((newSudoNode + root)%world_size), 0, comm);
			}
		}
		else if(sudoNode < (test * 2))
		{
			MPI_Recv(x, 1, MPI_INT, MPI_ANY_SOURCE, 0, comm, &status);
		}

		test = test * 2;
	}



}

int add_all(int x, MPI_Comm comm)
{




}

















