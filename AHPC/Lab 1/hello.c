#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv){
        int nprocs, mypid;

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
        MPI_Comm_rank(MPI_COMM_WORLD, &mypid);
        printf("Hello world from process %d of total %d!\n",mypid, nprocs);

        MPI_Finalize();
}
