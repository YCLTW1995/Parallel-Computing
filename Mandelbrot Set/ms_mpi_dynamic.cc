#define PNG_NO_SETJMP
#define DIETAG     2

#include <assert.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <iostream>

using namespace std ;


int *image ;
int num_threads ; 
//double left ;
//double right ;
double lower ;
double upper ;
int width ;
int height ;
const char* filename ;
int * workingstatus  ;



void write_png(const char* filename, const int width, const int height, const int* buffer) {
    FILE* fp = fopen(filename, "wb");
    assert(fp);
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    assert(png_ptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    assert(info_ptr);
    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    size_t row_size = 3 * width * sizeof(png_byte);
    png_bytep row = (png_bytep)malloc(row_size);
    for (int y = 0; y < height; ++y) {
        memset(row, 0, row_size);
        for (int x = 0; x < width; ++x) {
            int p = buffer[(height - 1 - y) * width + x];
            row[x * 3] = ((p & 0xf) << 4);
        }
        png_write_row(png_ptr, row);
    }
    free(row);
    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}
void cal(int begin, double left , double right  ) ;


int main(int argc, char** argv) {
    /* argument parsing */
    assert(argc == 9);
    int size, rank ;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status ;
    num_threads = strtol(argv[1], 0, 10);
    double left = strtod(argv[2], 0);
    double right = strtod(argv[3], 0);
    lower = strtod(argv[4], 0);
    upper = strtod(argv[5], 0);
    width = strtol(argv[6], 0, 10);
    height = strtol(argv[7], 0, 10);
    filename = argv[8];
    int isdone = 0 ;
    workingstatus = new int [height] ;
    for(int i = 0 ; i < height ; i ++ ){
        workingstatus[i] = 0 ; // 0 represent not finish 
    }
    /* allocate memory for image */
    image = (int*)malloc(width * height * sizeof(int));
    assert(image);
    int whichy = 0 ;
    int * masterworking = new int [2] ;// for master 
    int * workingitem = new int [2]  ;  //  for slave [0] for slave doing which y , [1] for which rank is doing 
    //[2] is for is done or not 
    //cout << "Rank "<< rank << "  START"<< endl ;
    if(rank == 0 ) {
        int whichdone = 0  ;
        int nowworking = 0 ; // for rank 0 assign 
        for(int allocate = 1 ; allocate < size ; allocate ++ ){
            masterworking[0] = allocate ;
            masterworking[1] = allocate ;
            MPI_Send(&masterworking[0] , 2 , MPI_INT , allocate , 0 , MPI_COMM_WORLD) ;
        }
        for(int x = 1 ; x < size ; x ++ ){

            MPI_Recv(&masterworking[0] , 2 , MPI_INT , x, 1 , MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
            MPI_Recv(&image[masterworking[0]*width] , width , MPI_INT , x , 2 , MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
        }
        nowworking = size -1 ; 
        while(isdone == 0  ){

            //MPI_Recv(&masterworking[0] , 2 , MPI_INT , MPI_ANY_SOURCE, 1 , MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            workingstatus[masterworking[0]] = 1 ;
            nowworking ++ ;
            if(workingstatus[height-1] == 1 ){
                isdone = 1 ;
                for(int x = 1 ; x < size ; x ++ ){
                    MPI_Send(&masterworking[0] , 2 , MPI_INT ,x, 3  , MPI_COMM_WORLD) ;
                }
                break ;
            }
            else {
                masterworking[0] = nowworking ;
                
                MPI_Send(&masterworking[0] , 2 , MPI_INT , masterworking[1]  , 0 , MPI_COMM_WORLD) ;
                MPI_Recv(&masterworking[0] , 2 , MPI_INT , MPI_ANY_SOURCE, 1 , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&image[masterworking[0]*width] , width , MPI_INT , masterworking[1] , 2 , MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
            }
        }
    }
    else{
        MPI_Recv(&workingitem[0], 2 , MPI_INT , 0 , 0 , MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
        whichy = workingitem[0] ;
        cal(whichy, left , right ) ;

        MPI_Send(&workingitem[0], 2  , MPI_INT, 0,1 , MPI_COMM_WORLD);
        MPI_Send(&image[whichy*width] , width , MPI_INT , 0 , 2 , MPI_COMM_WORLD) ;
        
        while(isdone == 0 ){
          //  MPI_Send(&workingitem[0], 2  , MPI_INT, 0,1 , MPI_COMM_WORLD);
            MPI_Recv(&workingitem[0], 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if(status.MPI_TAG == 3 ){
                isdone = 1 ;
            }
            else{
                whichy = workingitem[0] ;
                cal(whichy, left , right ) ;
                MPI_Send(&workingitem[0], 2  , MPI_INT, 0,1 , MPI_COMM_WORLD) ;
                MPI_Send(&image[whichy*width] , width , MPI_INT , 0 , 2 , MPI_COMM_WORLD) ;
            }
        }
        
    }

    /*
    //  mandelbrot set 
    for (int j = 0; j < height; ++j) {
    double y0 = j * ((upper - lower) / height) + lower;
    for (int i = 0; i < width; ++i) {
    double x0 = i * ((right - left) / width) + left;

    int repeats = 0;
    double x = 0;
    double y = 0;
    double length_squared = 0;
    while (repeats < 100000 && length_squared < 4) {
    double temp = x * x - y * y + x0;
    y = 2 * x * y + y0;
    x = temp;
    length_squared = x * x + y * y;
    ++repeats;
    }
    image[j * width + i] = repeats;
    }
    }
    */
    if(rank == 0 ){
        cal(0, left , right) ;
        write_png(filename, width, height, image);
    }
    /* draw and cleanup */
    //write_png(filename, width, height, image);
    free(image);
    MPI_Finalize() ;
    return 0 ;
}
void cal(int begin, double left , double right){

    for (int j = begin; j < begin+1; ++j) {
        double y0 = j * ((upper - lower) / height) + lower;
        for (int i = 0; i < width; ++i) {
            double x0 = i * ((right - left) / width) + left;

            int repeats = 0;
            double x = 0;
            double y = 0;
            double length_squared = 0;
            while (repeats < 100000 && length_squared < 4) {
                double temp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = temp;
                length_squared = x * x + y * y;
                ++repeats;
            }
            image[j * width + i] = repeats;
        }
    }


}

