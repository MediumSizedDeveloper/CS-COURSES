#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv){
    int nprocs, mypid, N, localCount, globalCount;
    N=100000;
    double x[N];
    double y[N];
    srand(time(NULL));
    
    //num of randomly generated points
    localCount=0;
    globalCount=0;
    MPI_Init(&argc, &argv);

    //get rank and size
    MPI_Comm_rank(MPI_COMM_WORLD, &mypid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    
    if(mypid==0){
	//RAND_MAX is a constant specifying the max value that 
	//can be returned by random() or rand().
	//By dividing by it, the random value will be between 0 and 1
	for(int i=0;i<N;i++){
	    x[i] = (double)random()/RAND_MAX;
	    y[i] = (double)random()/RAND_MAX;
	}
	//NOTICE: Bcast local to proc 0, will this cause error?
	printf("Broadcasting from proc 0"); 

    }

    MPI_Bcast(x,N,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(y,N,MPI_DOUBLE,0,MPI_COMM_WORLD);	
 
    double z;
    // WARNING: potential data loss due to integer division, 
    // N/nprocs must be perfectly divisible
    for(int i = mypid * (N/nprocs); i < (mypid+1) * (N/nprocs); i++){
	z = sqrt( x[i]*x[i] + y[i]*y[i] );
	if(z <= 1)
	    localCount++;
    }

    // NOTICE: Reduce local to all procs other than 0, error?
    MPI_Reduce(&localCount,&globalCount,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    printf("\n%d is the local count of proc %d\n",localCount,mypid);
    

    if(mypid==0){
	// calculate pi with our points, multiplying by 4 because relationship
	// between area of cirle/square
	double pi = (double)globalCount/(double)N * 4.0;

	printf("The value of pi is: %f", pi);
    }

    MPI_Finalize();
}
