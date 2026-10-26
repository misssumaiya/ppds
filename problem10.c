#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size,item;

    int data[8]={10,20,30,40,50,60,70,80};

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size!=3){
        if(rank==0){
            printf("use 3 process\n");
        }
        MPI_Finalize();
        return 0;
    }

    if(rank==0){
        for(int i=0; i<8; i++){
            int consumer=(i%2)+1;
            item=data[i];

            printf("P0 produces : %d\n",item);
            MPI_Send(&item,1,MPI_INT,consumer,0,MPI_COMM_WORLD);
        }

        item=-1;
        MPI_Send(&item,1,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Send(&item,1,MPI_INT,2,0,MPI_COMM_WORLD);
    }else{

        while(1){
            MPI_Recv(&item,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            if(item==-1)
            break;

            printf("p%d consumed: %d\n", rank,item);
        }
    }

    MPI_Finalize();
    return 0;
}