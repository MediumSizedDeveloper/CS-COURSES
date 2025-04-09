#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char **argv){
        
    int nprocs, mypid, N, index, count;
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
    int temp,temp2;
    for(int i = log2(nprocs)-1, j = 1; i >= 0; i--, j++){
	temp = (int)pow(2,i);
        if((mypid & (temp-1)) == 0){
	    index = N/pow(2,j);
            count = N/pow(2,j);
	    temp2 = (int)pow(2,i+1);
	    if((mypid & (temp2-1)) == 0){
	        MPI_Send(&arr[index], count, MPI_INT, mypid ^ temp, 0, MPI_COMM_WORLD);
                printf("Processor %d sneds sublist to processor %d at level %d\n",mypid,mypid ^ temp,j);
	    }
	    else{
	        MPI_Recv(arr, count, MPI_INT, mypid ^ temp, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                printf("Processor %d receives sublist from processor %d at level %d\n",mypid, mypid ^ temp,j);
	    }
	}
    }

    //compute partial sum
    int partialSum = 0;
    for(int i=0; i<N/nprocs; i++){
	partialSum += arr[i];
    }
    printf("The partial sum on processor %d is %d\n",mypid,partialSum); 

    //combing phase
    int receivedNum;
    for(int i = 0; i<log2(nprocs); i++){
	temp = (int)pow(2,i);
	if((mypid & (temp-1)) == 0){
	    temp2 = (int)pow(2,i+1);
	    if((mypid & (temp2-1)) != 0){
		MPI_Send(&partialSum,1,MPI_INT,mypid ^ temp,0,MPI_COMM_WORLD);
		printf("Processor %d sent partialSum %d to %d at level %d\n",mypid,partialSum,mypid^temp,i);
	    }
	    else{
		MPI_Recv(&receivedNum,1,MPI_INT,mypid  ^ temp,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		printf("Processor %d received partialSum %d from %d at level %d\n",mypid,receivedNum,mypid^temp,i);
		partialSum += receivedNum;
	    }
	}
    }

    if(mypid == 0){
	printf("The final sum is %d\n", partialSum);
    }
    MPI_Finalize();	
}
