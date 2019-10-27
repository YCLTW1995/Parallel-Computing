#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std ;
double tSTART = 0 , tEND = 0 , tIO = 0 , tCOMM = 0  , tTMP = 0 ;
int main (int argc , char*argv[]){
	int size , rank ;
	int N = atoi(argv[1]) ; // number of input size 
	float *arr = new float [N] ;
	MPI_File input, output ;
	MPI_Init(&argc,&argv) ;
	tSTART = MPI_Wtime() ;
	MPI_Comm_size(MPI_COMM_WORLD, &size) ;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank ) ;
	MPI_File_open(MPI_COMM_WORLD, argv[2], MPI_MODE_RDONLY, MPI_INFO_NULL,&input);
	MPI_File_open(MPI_COMM_WORLD, argv[3], MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &output ) ;
	if (N <= size  || size == 1 ) {
		tTMP = MPI_Wtime() ;
		MPI_File_read_at_all(input, (MPI_Offset)((0)*sizeof(float)), arr, N, MPI_FLOAT, MPI_STATUS_IGNORE);
		MPI_File_close(&input) ;
		tIO += MPI_Wtime() - tTMP ;
		if(rank == 0 ) {
			sort(arr, arr+N) ;	
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
	else { // start mpi version 
		int eachnum = N / size ;
		int rest = N % size ;
		int startpos = rank * eachnum ;
		if(rank == size - 1 ) eachnum += rest ;
		float *ans = new float [N] ;
		int *displs = new int [size] ;
		int *outeach = new int [size] ;
		for (int i = 0 ; i < size ; i ++ ){
			outeach[i] = N/size ;
			displs[i] = i*eachnum ;
		}
		outeach[size-1] += rest ;
		tTMP = MPI_Wtime() ;
		MPI_File_read_at(input, (MPI_Offset)((startpos)*sizeof(float)), arr , eachnum, MPI_FLOAT, MPI_STATUS_IGNORE);
		MPI_File_close(&input);
		tIO += MPI_Wtime() - tTMP ;
		float *recvarr = new float [N] ;
		float *original = new float [N] ;
		float *sortarr = new float[N] ;
		int isdone = 0 ;
		int iteration = 0 ;
		while(isdone == 0 ) {
			if(iteration % 2 == 0 ){ // even start 
				for(int i = 0 ; i < size -1 ; i = i + 2  ){

					tTMP = MPI_Wtime() ;
					if(i == rank){ // i proc send to i+1 proc
						MPI_Send(&arr[0],outeach[i]  , MPI_FLOAT, rank + 1 ,0 , MPI_COMM_WORLD) ;

					}
					else if(i+1 == rank ){ // i+1 proc recv 
						MPI_Recv(&recvarr[0], outeach[i] , MPI_FLOAT, rank-1 , 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					}
					tCOMM += MPI_Wtime() - tTMP ;
					if(i+1 == rank){
						for(int x = 0 ; x < eachnum ; x ++ ){
							original [x] = arr[x] ;
						}
						for (int x = 0 ; x < outeach[i] + outeach[i+1] ; x ++ ){
							if(x < outeach[i] ){
								sortarr[x] = recvarr[x] ;
							}
							else {
								sortarr[x] = original[x-outeach[i]] ;
							}
						}
						sort(sortarr, sortarr + outeach[i]+outeach[i+1]) ;
						for(int x = 0 ; x < eachnum ; x ++ ){
							arr[x] = sortarr[x+outeach[i]] ;
						}
					}
				}
				for (int i = 1 ; i < size ; i = i + 2  ){
					tTMP = MPI_Wtime() ;
					if(rank == i ){
						MPI_Send(&sortarr[0],outeach[i-1]  , MPI_FLOAT, rank - 1 ,0 , MPI_COMM_WORLD) ;
					}
					else if(i-1 == rank ){
						MPI_Recv(&recvarr[0],outeach[i-1], MPI_FLOAT, rank+1 , 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					}
					tCOMM += MPI_Wtime() -tTMP ;
					if(i-1 == rank ){
						for (int x = 0 ; x < outeach[i-1] ; x ++ ){
							arr[x] = recvarr[x] ;
						}
					}
				}
			}
			else if(iteration % 2 != 0 ) {
				for(int i = 1 ; i < size-1 ; i = i + 2 ){
					tTMP = MPI_Wtime() ;
					if(i == rank){ // i proc send to i+1 proc
						MPI_Send(&arr[0],outeach[i]  , MPI_FLOAT, rank + 1 ,0 , MPI_COMM_WORLD) ;
					}
					else if(i+1 == rank ){ // i+1 proc recv 
						MPI_Recv(&recvarr[0], outeach[i] , MPI_FLOAT, rank-1 , 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					}
					tCOMM += MPI_Wtime() - tTMP ;
					if(i+1 == rank){
						for(int x = 0 ; x < eachnum ; x ++ ){
							original [x] = arr[x] ;
						}
						for (int x = 0 ; x < outeach[i] + outeach[i+1] ; x ++ ){
							if(x < outeach[i] ){
								sortarr[x] = recvarr[x] ;
							}
							else {
								sortarr[x] = original[x-outeach[i]] ;
							}
						}
						sort(sortarr, sortarr +outeach[i]+outeach[i+1] );
						for(int x = 0 ; x < eachnum ; x ++ ){
							arr[x] = sortarr[x+outeach[i]] ;
						}
					}
				}
				for (int i = 2 ; i <  size ;i = i + 2  ){
					tTMP = MPI_Wtime() ;
					if(rank == i ){
						MPI_Send(&sortarr[0],outeach[i-1]  , MPI_FLOAT, rank - 1 ,0 , MPI_COMM_WORLD) ;
					}
					else if(i-1 == rank ){
						MPI_Recv(&recvarr[0], outeach[i-1] , MPI_FLOAT, rank+1 , 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					}
					tCOMM += MPI_Wtime() -tTMP ; 
					if( i-1 == rank ){
						for (int x = 0 ; x < outeach[i-1] ; x ++ ){
							arr[x] = recvarr[x] ;
						}
					}
				}
			}
			MPI_Barrier(MPI_COMM_WORLD);
			iteration ++ ;
			//if(iteration >= N ) isdone = 1 ;
			if(iteration % size == 0 ){
				tTMP = MPI_Wtime() ;
				MPI_Gatherv(&arr[0] , eachnum , MPI_FLOAT, &ans[0] ,outeach,displs,  MPI_FLOAT,0,MPI_COMM_WORLD) ;
				tCOMM += MPI_Wtime() - tTMP ;
				int checkdone = 1 ;
				if(rank == 0 ){
					for(int ii = 0 ; ii < N-1 ; ii ++ ){
						if(ans[ii] > ans[ii+1]){
							checkdone = 0 ;
							break ;
						}
					}
				}
				
				MPI_Bcast (&checkdone, 1, MPI_INT, 0, MPI_COMM_WORLD) ;
				if(checkdone == 1 ) isdone = 1 ;

			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
		if ( rank == 0 ) cout << "done "<< endl ;
		tTMP = MPI_Wtime() ;
		MPI_File_write_at(output, (MPI_Offset)((startpos)*sizeof(float)), arr , eachnum, MPI_FLOAT, MPI_STATUS_IGNORE);
		MPI_File_close(&output);
		tIO += MPI_Wtime() -tTMP ;

	}
	if(rank == 0 ){
		tEND = MPI_Wtime() ;
		cout << "TOTAL TIME : " << tEND - tSTART << endl ;
		cout << "IO    TIME : " << tIO << endl ;
		cout << "COMM  TIME : " << tCOMM << endl ;
		cout << "EXECU TIME : " << tEND- tSTART - tIO - tCOMM << endl ;

	}
	MPI_Finalize() ;
	return 0 ;
}
