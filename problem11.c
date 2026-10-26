#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size;
    int iterations=1000;
    int threshold=50;
    int block_size=256;
    int config[3];

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size !=4){
        if(rank==4){
            printf("run with 4 processes\n");
        }
        MPI_Finalize();
        return 0;
    }

    if(rank==0){
        config[0]=iterations;
        config[1]=threshold;
        config[2]=block_size;
    }
    MPI_Bcast(config,3,MPI_INT,0,MPI_COMM_WORLD);

    printf("P%d received iterartions= %d, Threshold=%d, Block size= %d\n",rank,config[0],config[1],config[2]);

    MPI_Finalize();
    return 0;
}