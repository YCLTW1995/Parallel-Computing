#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <mpi.h>

using namespace std ;

const int INF = 1000;
const int V = 2000;
void input(char* inFileName);
void output(char* outFileName);

void seq_FW();

int n, m;  // Number of vertices, edges
int Dist[V][V];

int main(int argc, char* argv[]) {
    input(argv[1]);
    int rank , size ;
    int numproc = atoi(argv[3]) ;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status; 
    MPI_Request request ;
    int realV = n ;
    int eachdist[realV] ;
    int connect[realV] ;
    int senddist[realV] ;
    int recvdist[realV] ;
    int checkallchange[size] ;
    for(int v = 0 ; v < realV ; v ++ ){
        connect[v] = -1 ;
    }
    int i = rank ;
    int connectcnt = 0 ;
    for(int j = 0 ; j < realV ; j ++ ){
        eachdist[j] = Dist[i][j] ; 
        if(eachdist[j] != 0 && eachdist[j] != INF){
            connect[connectcnt] = j ;
            connectcnt ++ ;
        }
    }
    int ischange = 0 ; // 1 represent this rank has updated
    int isdone = 0 ; // 0 represent is not done , 1 rerpesent all done 
    //cout <<"Before " <<endl ; 
    while(isdone == 0 ){
        ischange = 0 ;
        int tokenchange = 0 ; //0 means keep white , 1 means change black 
        for(int connectnum = 0 ; connectnum < connectcnt ; connectnum ++ ){ // connect vertex send

            for(int j = 0 ; j < realV ; j ++ ){
                int thisedge = Dist[i][connect[connectnum]] ;
                senddist[j] = eachdist[j] + thisedge ;
            }
            MPI_Isend(&senddist[0], realV, MPI_INT, connect[connectnum], 0 , MPI_COMM_WORLD, &request) ;
            //MPI_Irecv(&distrecv[0], realV , MPI_INT, connect[connectnum], 0,MPI_COMM_WORLD,&request2);
            MPI_Wait(&request, &status) ;

        }
        // MPI_Barrier(MPI_COMM_WORLD );
        for(int connectnum = 0 ; connectnum < connectcnt ; connectnum ++ ){
            MPI_Recv(&recvdist[0], realV, MPI_INT, connect[connectnum],0, MPI_COMM_WORLD, MPI_STATUS_IGNORE ) ;
            //MPI_Irecv(&distrecv[0], realV , MPI_INT, connect[connectnum], 0,MPI_COMM_WORLD,&request2);
            for(int j = 0 ; j < realV; j ++ ){
                if(recvdist[j] < eachdist[j] ){
                    ischange = 1 ;
                    eachdist[j] = recvdist[j] ;
                    if(rank < connect[connectnum]){
                        tokenchange = 1 ;
                    }
                }   
            }
        }
        MPI_Barrier(MPI_COMM_WORLD) ;
        //int token = 0 ; // 0 is white , 1 is black
        //if(rank == 0 ) cout << "BEGIN"<<endl ;
        int recvtoken = 0 ; // 0 is white , 1 is black 
        if(rank == 0 ) MPI_Send(&tokenchange, 1 , MPI_INT,1, 1, MPI_COMM_WORLD) ;
        for(int rk = 1 ; rk < size-1 ; rk ++ ){
            if(rank == rk ){
                MPI_Recv(&recvtoken, 1 , MPI_INT, rk -1 , 1 , MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
                if(recvtoken == 1 ) tokenchange = 1 ;
                MPI_Send(&tokenchange , 1 , MPI_INT, rk + 1 , 1 , MPI_COMM_WORLD);
            }
        }
        //cout << "MIDDONE"<<rank << endl ;
        //if(rank == 1 ) cout <<"!!!!!!!!!1"<<endl; 
        if(rank == size - 1 ){
            MPI_Recv(&recvtoken , 1 , MPI_INT , size - 2 , 1 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
            if(recvtoken == 1 ) tokenchange = 1 ;
            MPI_Send(&tokenchange, 1 , MPI_INT , 0 , 1 , MPI_COMM_WORLD);
        }
        //cout << "BEFORE0RECV"<< rank << " "<< endl ;
        if(rank == 0 ){
            MPI_Recv(&recvtoken, 1 , MPI_INT , size-1 , 1 , MPI_COMM_WORLD , MPI_STATUS_IGNORE) ;
        }
        MPI_Barrier(MPI_COMM_WORLD) ;
        MPI_Bcast(&recvtoken , 1 , MPI_INT , 0 , MPI_COMM_WORLD) ;
        if(recvtoken == 0 ) isdone = 1 ;
        //if(rank == 0 ) cout << "ISDONE"<<endl ;

        //cout <<"B ALL  " <<endl ;
    /*    MPI_Allgather(&ischange, 1 , MPI_INT, &checkallchange[0], 1 , MPI_INT,MPI_COMM_WORLD) ;
        int checkdone = 0 ; // 0 represent all done 
        for(int xx = 0 ; xx < size ; xx ++ ){
            if(checkallchange[xx] == 1 ) checkdone = 1 ;
        }
        if(checkdone == 0 ) isdone = 1 ;
        MPI_Barrier(MPI_COMM_WORLD);
        //cout <<"ITDONE"<<endl; 
   */
    }
    int out[realV*realV] ;
    MPI_Gather(&eachdist[0], realV, MPI_INT, &out[rank*realV], realV, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0 ) {
        for(int xx = 0 ; xx < realV; xx ++ ){
            for(int yy = 0 ; yy < realV; yy ++ ){
                Dist[xx][yy] = out[xx*realV+yy] ;
            }
        }
        output(argv[2]) ;
    }

    //seq_FW();
    MPI_Finalize() ;
    //output(argv[2]);
    return 0;
}

void input(char* inFileName) {
    FILE* infile = fopen(inFileName, "r");
    fscanf(infile, "%d %d", &n, &m);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j)
                Dist[i][j] = 0;
            else
                Dist[i][j] = INF;
        }
    }

    while (--m >= 0) {
        int a, b, v;
        fscanf(infile, "%d %d %d", &a, &b, &v);
        Dist[a][b] = v;
        Dist[b][a] = v;
    }
}

void output(char* outFileName) {
    FILE* outfile = fopen(outFileName, "w");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (Dist[i][j] >= INF)
                fprintf(outfile, "INF ");
            else
                fprintf(outfile, "%d ", Dist[i][j]);
        }
        fprintf(outfile, "\n");
    }
}

void seq_FW() {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (Dist[i][j] > Dist[i][k] + Dist[k][j]) {
                    Dist[i][j] = Dist[i][k] + Dist[k][j];
                }
            }
        }
    }
}
