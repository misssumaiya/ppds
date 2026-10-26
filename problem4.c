#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

int main(int argc,char**argv){

    int rank,size;
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size!=2){
        if(rank==0)
        printf("Use 2 processes only\n");

        MPI_Finalize();
        return 0;
    }

int sizes[]={64,256,1024,4096,16384,65536,1048576};

int iterations=5;
int count=5;

for(int k=0; k<count; k++){

    int bytes=sizes[k];
    
    char*data=malloc(bytes);

    if(data==NULL){
        printf("Memory allocation failed.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for(int i=0; i<bytes; i++)
    data[i]='A';

    MPI_Barrier(MPI_COMM_WORLD);

    double start=MPI_Wtime();

    for(int i=0; i<iterations;i++){
        if(rank==0){
            MPI_Send(data,bytes,MPI_CHAR,1,0,MPI_COMM_WORLD);
            MPI_Recv(data,bytes,MPI_CHAR,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }

        else{
             MPI_Recv(data,bytes,MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
             MPI_Send(data,bytes,MPI_CHAR,0,0,MPI_COMM_WORLD);
            }
        
    }

    double end=MPI_Wtime();
    if(rank==0){
        double total=end-start;
        double roundTrip=total/iterations;
        double oneWay=roundTrip/2.0;

        double badnwidth=(2.0*bytes*iterations)/total/1000000.0;

        printf("Message size: %d bytes\n",bytes);
        printf("Iterations: %d\n",iterations);
        printf("Average round_trip: %.9f seconds\n",roundTrip);
        printf("average one way: %.9f seconds\n",oneWay);
        printf("bandwidth: %.3f MB/s\n\n",badnwidth);
    }
    free(data);

}
MPI_Finalize();
return 0;
}