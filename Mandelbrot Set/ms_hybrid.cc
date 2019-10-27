#define PNG_NO_SETJMP
#include <mpi.h>
#include <assert.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <iostream>

using namespace std ;

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

int main(int argc, char** argv) {
    /* argument parsing */
    assert(argc == 9);
    int size, rank ;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_threads = strtol(argv[1], 0, 10);
    double left = strtod(argv[2], 0);
    double right = strtod(argv[3], 0);
    double lower = strtod(argv[4], 0);
    double upper = strtod(argv[5], 0);
    int width = strtol(argv[6], 0, 10);
    int height = strtol(argv[7], 0, 10);
    const char* filename = argv[8];
    int eachnum = 0 , eachstart = 0 ;
    int * eachnumarr = new int [size ] ;
    int * eachstartarr = new int [size] ;
    for (int i = 0 ; i < size; i ++ ){
        int tmpeach = height / size ;
        eachnumarr[i] = height/ size ;
        eachstartarr[i] = i * tmpeach ;
    }
    eachnumarr[size - 1 ] += height % size ;
    eachnum = height / size ;
    eachstart = rank  * eachnum ;
    if(rank == size - 1 )   eachnum += height % size ;

    /* allocate memory for image */
    int* image = (int*)malloc(width * height * sizeof(int));
    assert(image);
    int * ansarr = (int*) malloc(width * height * sizeof(int)) ;

    /* mandelbrot set */
    for (int j = eachstart ; j <eachstart + eachnum ; ++j) {

        double y0 = j * ((upper - lower) / height) + lower;
        #pragma omp parallel for num_threads(8) 
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
    for(int i = 1 ; i < size ; i ++ ){
        if(rank == i ){
            MPI_Send(&image[eachstartarr[i]* width ] , eachnumarr[i]*width ,MPI_INT,  0 , 0 , MPI_COMM_WORLD);
        }
        else if(rank == 0 ){
            MPI_Recv(&ansarr[eachstartarr[i]*width] , eachnumarr[i]*width , MPI_INT, i , 0 , MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD) ;
    /* draw and cleanup */
    if(rank == 0 ){
        for(int i = 0 ; i < eachnum*width ; i ++ ){
        
            ansarr[i] = image[i] ;
        }
        write_png(filename, width, height, ansarr);
    }
    free(image);
    MPI_Finalize() ;
    return 0 ;

}
