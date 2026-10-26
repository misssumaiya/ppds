#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size;
    int local,manual,prefix;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    local=(rank+1) *10;

    MPI_Scan(&local,&prefix,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    if(rank==0){
        manual=local;

    }else{
        int prev_prefix;

        MPI_Recv(&prev_prefix,1,MPI_INT,rank-1,5,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        manual=prev_prefix+local;

    }

    if(rank<size-1){
        MPI_Send(&manual,1,MPI_INT,rank+1,5,MPI_COMM_WORLD);
    }

    printf("p%d: Local value=%d ,prefix sum= %d\n ",rank,local,prefix);

    int results[size];

    MPI_Gather(&manual,1,MPI_INT,results,1,MPI_INT,0,MPI_COMM_WORLD);

    if(rank==0){
        printf("\nPrefix result: ");
        for(int i=0; i<size; i++){
            printf("%d ",results[i]);
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}