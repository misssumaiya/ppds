#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size;

    int A[2][2]={{1,2},{3,4}};
    int B[2][2]={{5,6},{7,8}};
    int row[2];
    int c[2][2];
    int result[2];

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size!=2){
        if(rank==0)
            printf("use 2 process\n");

        MPI_Finalize();
        return 0;
    }

    if(rank==0){
        row[0]=A[0][0];
        row[1]=A[0][1];

        result[0]=row[0]*B[0][0]+row[1]*B[1][0];
        result[1]=row[0]*B[0][1]+row[1]*B[1][1];

        c[0][0]=result[0];
        c[0][1]=result[1];

        MPI_Send(A[1],2,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Recv(c[1],2,MPI_INT,1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    }
    else{
        MPI_Recv(row,2,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        result[0]=row[0]*B[0][0]+row[1]*B[1][0];
        result[1]=row[0]*B[0][1]+row[1]*B[1][1];

        MPI_Send(result,2,MPI_INT,0,1,MPI_COMM_WORLD);

    }

    if(rank==0){

        printf("c[0][0]=1*5+2*7=%d\n",c[0][0]);
        printf("c[0][1]=1*6+2*8=%d\n",c[0][1]);
        printf("c[1][0]=3*5+4*7=%d\n",c[1][0]);
        printf("c[1][1]=3*6+4*8 =%d\n",c[1][1]);


        printf("\nMatrix C=AxB\n");

        for(int i=0; i<2; i++){
            for(int j=0; j<2; j++)
                printf("%d ",c[i][j]);

            
            printf("\n");
        }
    }
    MPI_Finalize();
    return 0;

}