#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv){
    int nprocs, mypid, N;
    N=1000; //array size
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &mypid);
    
    int arr[N];
    int partialSum=0;
    int globalSum=0;

    if(mypid==0){
			for(int i=0;i<N;i++)
	    arr[i]=i+1;
    }
		
		//tells every processor the same thing
    MPI_Bcast(arr,N,MPI_INT,0,MPI_COMM_WORLD);
    
    //tailored for loop for corresponding processor rank
    for(int i = mypid*(N/nprocs);i<(mypid+1)*N/nprocs;i++){
    	partialSum+=arr[i];
    }
    
    printf("%d: from process %d\n", partialSum, mypid);
    
    //sums each processors' partialSum value and stores the result in
    //globalSum, sending it back to processor 0
    
    MPI_Reduce(&partialSum,&globalSum,1,MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(mypid==0){
        printf("The global sum is %d\n",globalSum);
    }

    MPI_Finalize();
}
