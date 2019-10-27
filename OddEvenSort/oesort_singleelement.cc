#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std ;
double tSTART = 0 , tEND = 0 , tIO = 0 , tCOMM = 0  , tTMP = 0 ; 

int main (int argc , char * argv[])
{
    int rank , size ;
    int N = atoi(argv[1]) ; // Number of input size 
    float *arr = new float [N] ;
    MPI_File input, output ;
    MPI_Init(&argc,&argv) ;
    tSTART = MPI_Wtime() ; // mpi start time 
    MPI_Comm_size(MPI_COMM_WORLD, &size) ;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank ) ;
    MPI_File_open(MPI_COMM_WORLD, argv[2], MPI_MODE_RDONLY, MPI_INFO_NULL,&input);
    MPI_File_open(MPI_COMM_WORLD, argv[3], MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &output ) ;

    if(N<=size || size == 1 ){ // do seq way
        tTMP = MPI_Wtime() ;
        MPI_File_read_at_all(input, (MPI_Offset)((0)*sizeof(float)), arr, N, MPI_FLOAT, MPI_STATUS_IGNORE);
        MPI_File_close(&input) ;
        tIO += MPI_Wtime() - tTMP ;
        int iteration = 0 ;
        int isdone = 0 ;
        if(rank == 0 ){
            while(isdone == 0 ) {
                if(iteration % 2 == 0 ){
                    for (int j = 0 ; j < N-1 ; j=j+2){
                        if(arr[j] > arr[j+1] ) {
                            swap ( arr[j], arr[j+1]) ;
                        }
                    }
                }
                else {
                    for(int j = 1 ; j < N-1 ; j = j + 2 ){
                        if(arr[j] > arr[j+1]){
                            swap(arr[j], arr[j+1]) ;
                        }
                    }
                }
                iteration ++ ;
                int tmpcheck = 0 ;
                if( iteration %  N  == 0  ) { // check if it is done 
                    for (int j = 0 ; j < N -1 ; j ++ ){
                        if(arr[j] > arr[j+1]) {
                            tmpcheck = 1 ;
                            break ;
                        }
                    }
                    if ( tmpcheck == 0 ) isdone = 1 ;
                }

            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        tTMP = MPI_Wtime() ;
        MPI_Bcast(arr, N, MPI_FLOAT, 0, MPI_COMM_WORLD);
        tCOMM += MPI_Wtime() - tTMP ;
        tTMP = MPI_Wtime() ;
        MPI_File_write_at(output, (MPI_Offset)((0)*sizeof(float)), arr , N, MPI_FLOAT, MPI_STATUS_IGNORE);
        MPI_File_close(&output) ;
        tIO += MPI_Wtime() - tTMP ;
    }
    else { // do parallel way 
        int eachnum = N / size ;
        int eachoe = 0 ; // if eachnum is even eachoe=0 
        if(eachnum %2 != 0 ) eachoe = 1 ;
        int rest = N % size ;
        int startpos = rank * eachnum ;
        float originaltmp, recvtmp ;
        float *ans = new float [N] ;
        int *displs = new int [size] ;
        int *fuckeach = new int [size] ;
        for (int i = 0 ; i < size ; i ++ ){
            fuckeach[i] = N/size ;
            displs[i] = i*eachnum ;
        }
        fuckeach[size-1] += rest ;
        if(rank == size - 1 ) eachnum += rest ;
        tTMP = MPI_Wtime() ;
        MPI_File_read_at(input, (MPI_Offset)((startpos)*sizeof(float)), arr , eachnum, MPI_FLOAT, MPI_STATUS_IGNORE);
        MPI_File_close(&input);
        tIO += MPI_Wtime() - tTMP ;
        int isdone = 0 ;
        int iteration = 0 ;
        int ischange = 0 ;
        MPI_Barrier(MPI_COMM_WORLD);
        while(isdone == 0) {
            ischange = 0 ;
            if(iteration % 2 == 0 ){
                if(eachoe == 0){
                    for(int i = 0 ; i < eachnum -1  ; i = i + 2 ){
                        if(arr[i] > arr[i+1] ) {
                            // swap ( arr[i], arr[i+1]) ;
                            float ttt = arr[i+1] ;
                            arr[i+1] = arr[i] ;
                            arr[i] = ttt ;
                        }
                    }
                }
                else if(eachoe == 1 ){
                    int add = 0 ;
                    if (rank % 2 == 1 ) add = 1 ;
                    for (int i = add ; i < eachnum -1  ; i = i + 2 ){
                        if(arr[i] > arr[i+1] ) {
                            swap ( arr[i], arr[i+1]) ;
                        }
                    }
                    for (int i = 0 ; i < size - 1 ; i = i + 2 ){
                        tTMP = MPI_Wtime() ;
                        if(rank == i ) {
                            MPI_Send(&arr[eachnum-1], 1 , MPI_FLOAT, rank + 1 ,0 , MPI_COMM_WORLD) ;
                        }
                        else if(i+1 == rank ) {
                            MPI_Recv(&recvtmp, 1 , MPI_FLOAT, rank-1 , 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            originaltmp = arr[0] ;
                            if(recvtmp > arr[0]){
                                //   ischange = 1 ;
                                originaltmp = arr[0] ;
                                arr[0] = recvtmp ;
                            }
                        }
                        tCOMM += MPI_Wtime() - tTMP ;
                    }
                    for(int i = 1 ; i < size  ; i = i + 2 ){
                        tTMP = MPI_Wtime() ;
                        if(rank == i ) {
                            MPI_Send(&originaltmp, 1 , MPI_FLOAT, rank - 1 ,0 , MPI_COMM_WORLD) ;
                        }
                        else if(i-1 == rank ){
                            MPI_Recv(&recvtmp, 1 , MPI_FLOAT, rank + 1  , 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            if(arr[eachnum - 1 ] > recvtmp ){
                                arr[eachnum - 1 ] = recvtmp ;
                            }
                        }
                        tCOMM += MPI_Wtime() - tTMP ;
                    }
                }
                MPI_Barrier(MPI_COMM_WORLD);
            }
            else {   /////// if iteration is odd 
                if(eachoe == 0){
                    for(int i = 1 ; i < eachnum -1  ; i = i + 2 ){
                        if(arr[i] > arr[i+1] ) {
                            swap ( arr[i], arr[i+1]) ;
                        }
                    }
                    for (int i = 0 ; i < size - 1 ;  i ++ ){
                        tTMP = MPI_Wtime() ;
                        if(rank == i ) {
                            MPI_Send(&arr[eachnum-1], 1 , MPI_FLOAT, rank + 1 ,0 , MPI_COMM_WORLD) ;
                        }
                        else if(i+1 == rank ) {
                            MPI_Recv(&recvtmp, 1 , MPI_FLOAT, rank-1 , 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            originaltmp = arr[0] ;
                            if(recvtmp > arr[0]){
                                //   ischange = 1 ;
                                originaltmp = arr[0] ;
                                arr[0] = recvtmp ;
                            }

                        }
                        tCOMM += MPI_Wtime() -tTMP ;
                    }
                    MPI_Barrier(MPI_COMM_WORLD);
                    for(int i = 1 ; i < size  ;  i ++ ){
                        tTMP = MPI_Wtime() ;
                        if(rank == i ) {
                            MPI_Send(&originaltmp, 1 , MPI_FLOAT, rank - 1 ,0 , MPI_COMM_WORLD) ;
                        }
                        else if(i-1 == rank ){
                            MPI_Recv(&recvtmp, 1 , MPI_FLOAT, rank + 1  , 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            if(arr[eachnum - 1 ] > recvtmp ){
                                arr[eachnum - 1 ] = recvtmp ;
                            }
                        }
                        tCOMM += MPI_Wtime() - tTMP ;
                    }
                    MPI_Barrier(MPI_COMM_WORLD);
                }
                else if(eachoe == 1 ){
                    ///// normal oesort 
                    if(rank % 2 == 0 ) {
                        for(int i = 1 ; i < eachnum ; i = i + 2 ){
                            if(arr[i] > arr[i+1] ) {
                                swap ( arr[i], arr[i+1]) ;
                            }
                        }
                    }
                    else if(rank % 2 != 0 ) {
                        for(int i = 0 ; i < eachnum - 1 ; i = i + 2 ){
                            if(arr[i] > arr[i+1] ) {
                                swap ( arr[i], arr[i+1]) ;
                            }
                        }
                    }
                    //////start mpi oesort 
                    for(int i = 1 ; i < size -1 ; i = i + 2 ){
                        tTMP = MPI_Wtime() ;
                        if(rank == i  ){
                            MPI_Send(&arr[eachnum-1], 1 , MPI_FLOAT, rank + 1 ,0 , MPI_COMM_WORLD) ;    
                        }
                        else if(i + 1 == rank ){
                            MPI_Recv(&recvtmp, 1 , MPI_FLOAT, rank-1 , 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            originaltmp = arr[0] ;
                            if(recvtmp > arr[0]){
                                originaltmp = arr[0] ;
                                arr[0] = recvtmp ;
                            }
                        }
                        tCOMM += MPI_Wtime() - tTMP ;

                    }
                    for(int i = 2 ; i < size ; i = i + 2 ){
                        tTMP = MPI_Wtime() ;
                        if(rank == i ) {
                            MPI_Send(&originaltmp, 1 , MPI_FLOAT, rank - 1 ,0 , MPI_COMM_WORLD) ;
                        }
                        else if(i-1 == rank ){
                            MPI_Recv(&recvtmp, 1 , MPI_FLOAT, rank + 1  , 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            if(arr[eachnum - 1 ] > recvtmp ){
                                arr[eachnum - 1 ] = recvtmp ;
                            }
                        }
                        tCOMM += MPI_Wtime() - tTMP ;

                    }
                }
                MPI_Barrier(MPI_COMM_WORLD);
            }

            iteration ++ ;
            int check = N / 5 ;
            if( iteration > N/2 && iteration % check == 0 ){
                tTMP = MPI_Wtime() ;
                MPI_Barrier(MPI_COMM_WORLD);
                MPI_Gatherv(&arr[0] , eachnum , MPI_FLOAT, &ans[0] ,fuckeach,displs,  MPI_FLOAT,0,MPI_COMM_WORLD) ; 
                tCOMM = MPI_Wtime() - tTMP ;
                int checkdone = 1 ;
                if(rank == 0 ){
                    for (int xxx = 0 ; xxx < N-1 ; xxx ++ ){
                        if(ans[xxx] > ans[xxx+1]){
                            checkdone = 0 ;
                            break ;
                        }
                    }
                }
                MPI_Bcast (&checkdone, 1, MPI_INT, 0, MPI_COMM_WORLD) ;
                if(checkdone == 1 ) isdone = 1 ;

            }
            // if (iteration > N ) isdone = 1 ;
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if ( rank == 0 ) cout << "done "<< endl ;
        tTMP = MPI_Wtime() ;
        MPI_File_write_at(output, (MPI_Offset)((startpos)*sizeof(float)), arr , eachnum, MPI_FLOAT, MPI_STATUS_IGNORE);
        MPI_File_close(&output); 
        tIO += MPI_Wtime() - tTMP ;
    }
    if(rank == 0 ){
        tEND=MPI_Wtime() ;
        cout << "TOTAL TIME : " << tEND - tSTART << endl ;
        cout << "IO    TIME : " << tIO << endl ;
        cout << "COMM  TIME : " << tCOMM << endl ;
        cout << "EXECU TIME : " << tEND- tSTART - tIO - tCOMM << endl ;
    }
    MPI_Finalize() ;
    return 0 ;
}
