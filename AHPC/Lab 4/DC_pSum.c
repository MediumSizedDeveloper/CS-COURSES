#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char **argv){
        
	int nprocs, mypid, N, P;
	p = 8;
	N = 256;
	int arr[N];
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &mypid);
	
	//initialization
	if(mypid == 0){
		for(int i=0; i<N; i++)
			arr[i] = i+1;
	}
	
//dividing phase
	for(int i = log(P,2)-1, int j = 1; i >= 0; i--, j++){
	
		if(mypid & pow(2,i)-1 == 0){
			
			if(mypid & pow(2,i+1)-1 == 0){
				MPI_Send(&arr[N/pow(2,j)], N/pow(2,j), MPI_INT, mypid ^ pow(2,i), 0, MPI_COMM_WORLD);
			}
			else{
				MPI_Recv(arr, N/pow(2,j), MPI_INT, mypid ^ pow(2,i), 0, MPI_COMM_WORLD);
			}
		}
	}
	
//computing phase
	
}
