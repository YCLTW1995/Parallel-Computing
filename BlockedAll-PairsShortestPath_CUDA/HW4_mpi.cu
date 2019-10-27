#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cuda.h>
#include <algorithm>
#include <mpi.h>



using namespace std ;
// blocking factor
#define B 32

const int INF = 1000000000;
const int V = 20010;
void input(char* inFileName);
void output(char* outFileName);

void block_FW(int rank , int size );
int ceil(int a, int b);
void cal(int Round, int block_start_x, int block_start_y,
        int block_width, int block_height);

int n, m;  // Number of vertices, edges
//static int Dist[V][V];
static int dist[V*V] ;
__global__ void fuckcuda(int *garr , int * data )
{
    __shared__ int sdist[32*32];
    int round = data[0] ;
    int n = data[1] ;
    int x = threadIdx.x;
    int y = threadIdx.y ;
    int i = x + round * 32 ;
    int j = y + round * 32;
    if(i<n && j < n ){
        sdist[x * 32 + y] = garr[i*n+j];
    }
    else sdist[x * 32 + y] =1000000000;

    __syncthreads();
#pragma unroll
    for (int k = 0 ; (k < 32 )&&((round*32+k) < n) ; k ++){
        int tmp = sdist[ x* 32 + k] + sdist[k *32 + y];
        if (tmp < sdist[x *32 + y]) sdist[x *32 + y] = tmp;
        __syncthreads();

    }

    if (i < n && j < n) {

        garr[i*n+j] = sdist[x * 32 + y];

    } 

    __syncthreads();

}
__global__ void phase2_vertical (int * arr , int *data ) 
{
    //  int round = r ;
    int round = data[0] ;
    int n = data[1] ;
    int whichcol = blockIdx.y ;
    if(whichcol != round){

        __shared__ int TMP[64][64];
        __shared__ int DOING[64][64];
        int i = threadIdx.y;
        int j = threadIdx.x;
        int col = blockIdx.y * 32 + i;
        int row = round * 32 + j;
        int start = 32 * round ; 
        int end = 0 ;


        if(col >= n || row >= n){
            DOING[i][j] = INF;
        }else{
            DOING[i][j] = arr[col*n  + row];
        }
        int doingi = start + i;
        int doingj = start + j;
        if(doingi >= n || doingj >= n){
            TMP[i][j] = INF;
        }else{
            TMP[i][j] = arr[doingi*n + doingj] ;
        }

        __syncthreads();
        if(col >= n || row >= n) return;

        if (start + 32 < n ) end = 32 ;
        else end = n - start ;


        for(int k = 0; k < end ; k++ ){
            int check = DOING[i][k] + TMP[k][j] ;
            if(check < DOING[i][j]){
                DOING[i][j] = check ;
            }
        }
        arr[col*n +row] = DOING[i][j] ;
    }
    else {
        return ;
    }


}
__global__ void phase2_row(int * arr , int * data  )
{
    //     int round = r ;
    int round = data[0] ;
    int n = data[1] ;
    int whichrow = blockIdx.x ;
    if(whichrow != round){
        __shared__ int TMP[64][64];
        __shared__ int DOING[64][64];
        int i = threadIdx.y;
        int j = threadIdx.x;
        int col = round * 32 + i;
        int row = blockIdx.x * 32 + j;
        int start = 32 * round ; 
        int end = 0 ;


        if(col >= n || row >= n){
            DOING[i][j] = INF;
        }else{
            DOING[i][j] = arr[col*n  +row];
        }
        int doingi = start + i;
        int doingj = start + j;
        if(doingi >= n || doingj >= n){
            TMP[i][j] = INF;
        }else{
            TMP[i][j] = arr[doingi*n + doingj] ;
        }

        __syncthreads();
        if(col >= n || row >= n) return;

        if (start + 32 < n ) end = 32 ;
        else end = n - start ;

        for(int k = 0; k < end ; k++){
            int check = DOING[k][j] + TMP[i][k] ; 
            if(check < DOING[i][j] ) {
                DOING[i][j] = check ;
            }
        }
        arr[col*n +row ]= DOING[i][j] ;
    }
    else return ;
}

int main(int argc, char* argv[]) {

    int size , rank ;

    MPI_Init(&argc , &argv) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
    MPI_Comm_size(MPI_COMM_WORLD, &size ) ;

    input(argv[1]);
    /*
       for (int i = 0 ; i < n ; i ++ ){
       for (int j = 0 ; j < n ; j ++ ){
       dist[i*n+j] = Dist[i][j];
       }
       }*/
    block_FW(rank , size );
    if(rank == 0 ) {
        output(argv[2]);
    }
    MPI_Finalize() ;
    return 0;
}

void input(char* inFileName) {
    FILE* infile = fopen(inFileName, "r");
    fscanf(infile, "%d %d", &n, &m);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j)
                dist[i*n+j] = 0;
            else
                dist[i*n+j] = INF;
        }
    }

    while (--m >= 0) {
        int a, b, v;
        fscanf(infile, "%d %d %d", &a, &b, &v);
        dist[a*n+b] = v;
    }
}

void output(char* outFileName) {
    FILE *outfile = fopen(outFileName, "w");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // if (Dist[i][j] >= INF)	fprintf(outfile, "INF ");
            // else					fprintf(outfile, "%d ", Dist[i][j]);
            if (dist[i*n+j] >= INF)
                dist[i*n+j] = INF;


        }
        fwrite(&dist[i*n], sizeof( int), n, outfile);
    }
    fclose(outfile);
}

int ceil(int a, int b) { return (a + b - 1) / b; }
__global__ void Phase_3(int *device, int round, int n)
{
    if(blockIdx.x == round || blockIdx.y == round) return;
    int start = round*32;
    int end = ((start + 32) < n) ? 32 : n - start;
    int i = threadIdx.y;
    int j = threadIdx.x;
    int col = blockIdx.y * 32 + i;
    int row = blockIdx.x * 32 + j;
    __shared__ int Pi[32][32];
    __shared__ int Pj[32][32];
    if(col >= n || start + j >= n){
        Pi[i][j] = 1000000000 ;
    }
    else{
        Pi[i][j] = device[col*n + (start + j)];
    }
    if(row >= n || start + i >= n){
        Pj[i][j]  =  1000000000 ;
    }
    else {
        Pj[i][j] =  device[(start + i)*n + row];
    }
    //printf("fuck\n");
    if(row < n && col < n) {
        int check = device[col*n + row];
        __syncthreads();
        for(int k = 0; k < end ; ++k){
            check = min(check, Pi[i][k] + Pj[k][j]);
        }
        device[col*n + row] = check;
    }
    else return ;
}
void block_FW(int rank , int size ) {
    int round = ceil(n, B);
    //int r = 1 ;

    dim3 blocknum(1,1) ;
    dim3 blocksize(32,32) ;
    ssize_t arrsize = n*n*sizeof(int) ;

    int *data ;
    data = new int [2] ;

    int *garr ;
    int *gdata ;
    cudaMalloc(&garr, arrsize) ;
    cudaMalloc(&gdata , sizeof(int)*2) ;

    cudaMemcpy(garr,dist, arrsize,cudaMemcpyHostToDevice);

    //int *phase2ans1 , *phase2ans2 ;
   // phase2ans1 = new int [n*n ] ;
   // phase2ans2 = new int [n*n ] ;

    MPI_Barrier(MPI_COMM_WORLD) ;
    for (int r = 0; r < round; ++r) {

        /* Phase 1*/

        data[0] = r ;
        data[1] = n ;
    

        cudaMemcpy(gdata,data, sizeof(int)*2,cudaMemcpyHostToDevice);
        if(rank == 0 ){
   //       cudaMemcpy(garr,dist, arrsize,cudaMemcpyHostToDevice);
            fuckcuda<<<blocknum, blocksize>>>(garr , gdata);  
        }
        else if(rank == 1 ) {
   //       cudaMemcpy(garr,dist, arrsize,cudaMemcpyHostToDevice);
   //         fuckcuda<<<blocknum, blocksize>>>(garr , gdata);  
            
        }
    /*    
        if(rank == 1 ) {
            MPI_Send(&dist[pos] ,height * n , MPI_INT , 0 , 0 , MPI_COMM_WORLD ) ;    
        }
        else if(rank == 0 ) {
            MPI_Recv(&dist[pos] , height * n , MPI_INT , 1 , 0 , MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
        }
        */
    
        
        
        MPI_Barrier(MPI_COMM_WORLD);
       
        
        
        
        // MPI_Bcast( &dist[pos], height * n , MPI_INT, 1 , MPI_COMM_WORLD  ) ;
    //    MPI_Bcast(dist , n*n , MPI_INT , 0 , MPI_COMM_WORLD) ;
   //     cudaMemcpy(garr , dist , arrsize , cudaMemcpyHostToDevice);
        if(rank == 0 ) {
            
   //       cudaMemcpy(garr,dist, arrsize,cudaMemcpyHostToDevice);
   //         fuckcuda<<<blocknum, blocksize>>>(garr , gdata);  
            phase2_vertical<<<dim3(1,round), dim3(32,32)>>>(garr , gdata ) ;
            phase2_row<<<dim3(round,1), dim3(32,32)>>>(garr, gdata) ;
            cudaMemcpy(dist , garr , arrsize , cudaMemcpyDeviceToHost) ;
        }
        else if(rank == 1 ) {
    
   //       cudaMemcpy(garr,dist, arrsize,cudaMemcpyHostToDevice);
   //         fuckcuda<<<blocknum, blocksize>>>(garr , gdata);  
            phase2_row<<<dim3(round,1), dim3(32,32)>>>(garr, gdata) ;
            cudaMemcpy(dist , garr , arrsize , cudaMemcpyDeviceToHost) ;

        }
        MPI_Barrier(MPI_COMM_WORLD);
        

        /////Phase3 

        int start = r*32 ;
        int height = 32 ;
        if(start + 32 > n ) height = n - start ;
        int pos = start * n ;
        MPI_Bcast( &dist[pos], height * n , MPI_INT, 0 , MPI_COMM_WORLD  ) ;
    /*    
        if(rank == 1 ) {
            MPI_Send(&dist[pos] ,height * n , MPI_INT , 0 , 0 , MPI_COMM_WORLD ) ;    
        }
        else if(rank == 0 ) {
            MPI_Recv(&dist[pos] , height * n , MPI_INT , 1 , 0 , MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
        }
        */
        MPI_Barrier(MPI_COMM_WORLD);
       // MPI_Bcast( &dist[pos], height * n , MPI_INT, 1 , MPI_COMM_WORLD  ) ;
    //    MPI_Bcast(dist , n*n , MPI_INT , 0 , MPI_COMM_WORLD) ;
   //     cudaMemcpy(garr , dist , arrsize , cudaMemcpyHostToDevice);
        if(rank == 0 ) {
        //    cudaMemcpy(garr , dist , arrsize , cudaMemcpyHostToDevice);
            Phase_3<<<dim3(round, round), dim3(32, 32)>>>(garr, r, n);
        }
        MPI_Barrier(MPI_COMM_WORLD) ;

    }
    if(rank == 0 ){
        cudaMemcpy(dist, garr, arrsize, cudaMemcpyDeviceToHost);
    }
    cudaFree(garr);
    cudaFree(gdata) ;
    delete[] data ;
    MPI_Barrier(MPI_COMM_WORLD);
}

