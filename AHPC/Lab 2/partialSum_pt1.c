#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv){
        
	int nprocs, mypid, N;
	N=1000;

	MPI_Init(&argc, &argv);
	
	//get the pid
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &mypid);
	int partial_sum=0;
	
	if(mypid == 0){
		int arr[N];
		int total_Sum=0;
		
		for(int i=0; i<N; i++){
			arr[i]=i+1;
		}

		for(int i=1; i<nprocs; i++){
			MPI_Send(&arr[i*100],100,MPI_INT,i,0, MPI_COMM_WORLD);
		}
		
		for(int i=0; i<N/nprocs; i++){
			total_Sum += arr[i];
		}
		printf("Partial sum from processor %d of total %d processors is: %d\n", mypid, nprocs, total_Sum);

		for(int i=1; i<nprocs; i++){
			MPI_Recv(&partial_sum,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			total_Sum += partial_sum;
			printf("Received %d from processor %d\n", partial_sum,i);
			printf("Partial sum from processor %d of total %d processors is:%d\n",i,nprocs,total_Sum);
		}
		printf("Total sum is :%d",total_Sum);	
	}
	else if(mypid != 0){
                int partial_Arr[N/nprocs];
                MPI_Recv(partial_Arr,N/nprocs,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

                for(int i=0; i<N/nprocs; i++){
                        partial_sum += partial_Arr[i];
                }
                MPI_Send(&partial_sum,1,MPI_INT,0,0,MPI_COMM_WORLD);
                printf("Process %d sent their sum: %d to process 0\n",mypid,partial_sum);
        }
        MPI_Finalize();
}	
