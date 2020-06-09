#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <immintrin.h>

#define CHANNEL 3
#define HEIGHT 24
#define WIDTH 16
// for this example, kernel width and height are both 4
// Also, to simulate the code in Fast Convolution, the output channel is one
//	(in Fast Convolution, the output channel is seperated caculated into for loop)
struct nz_info{
	int c;
	int h;
	int w;
	double value ;	
};

void Generate_Input(std::vector<std::vector<std::vector<nz_info> > > &v0_nz_vec, 
		std::vector<std::vector<nz_info> > &kernel)
{
	printf("GENERATING INPUT\n");
	for(int c = 0 ; c < CHANNEL ; c ++ ){
		for(int h = 0 ; h < HEIGHT ; h ++){
			for(int w = 0 ; w < WIDTH ; w ++) {
				nz_info img_info ;
				img_info.c = c ;
				img_info.h = h ;
				img_info.w = w ;
				double temp_r = rand()%255;
				img_info.value = temp_r ;
				v0_nz_vec[c][h].push_back(img_info);

			}
		}
	}
	printf("INPUT IMAGE DIMENSION IS %d*%d*%d\n", CHANNEL, HEIGHT, WIDTH);
	// kernel dimension is 4*4
	for(int i = 0 ; i < 4 ; i ++){
		for(int j = 0 ; j < 4 ; j ++ ){
			nz_info kernel_info ;
			kernel_info.c = 0 ;
			kernel_info.h = i ;
			kernel_info.w = j ;
			double temp_r = rand()%3 ;
			temp_r = temp_r -1 ;
			kernel_info.value = temp_r ;
			kernel[i].push_back(kernel_info) ;
		}
	}

	printf("KERNEL DIMENSION is %d*%d*%d\n",1,4,4);
}

void Convolution(std::vector<std::vector<std::vector<nz_info> > > &v0_nz_vec, 
		std::vector<std::vector<nz_info> > &kernel,
		int ***output)
{
	// in this example, stide is one ->no need to padding
	int resultH = HEIGHT - 4 + 1 ;
	int resultW = WIDTH - 4 + 1 ;
	printf("Output Dimension is  %d*%d*%d\n", 1,resultH, resultW);
	int output_channel = 1 ;

	output = new int **[output_channel] ;
	for(int i = 0 ; i < output_channel ; i ++){
		output[i] = new int * [resultH];
		for(int j = 0 ; j < resultH ; j ++){
			output[i][j] = new int [resultW] ;
		}
	}
	clock_t conv_start, conv_end ;
	clock_t load_t_start, load_t_end ;
	double load_t = 0 ;
	conv_start = clock() ;
	for(int OC = 0 ; OC < output_channel ; OC++){
		for(int chan = 0 ; chan < CHANNEL ; chan ++){
			for(int rh = 0 ; rh < resultH ; rh ++){
				for(int rw = 0 ; rw < resultW ; rw ++){
					int channel_partial_sum = 0 ;
					//original c version
					/*
					for(int k = 0 ; k < 16 ; k ++){
						channel_partial_sum += v0_nz_vec[chan][rh+k/4][rw+k%4].value*kernel[k/4][k%4].value ;
					}
					output[OC][rh][rw] += channel_partial_sum ;
					*/
					// AVX version
					load_t_start = clock() ;
					__m256i v0_part1 = _mm256_setr_epi32((int) v0_nz_vec[chan][rh+0][rw+0].value,
				(int) v0_nz_vec[chan][rh+0][rw+1].value, (int) v0_nz_vec[chan][rh+0][rw+2].value,
				(int) v0_nz_vec[chan][rh+0][rw+3].value, (int) v0_nz_vec[chan][rh+1][rw+0].value,
				(int) v0_nz_vec[chan][rh+1][rw+1].value, (int) v0_nz_vec[chan][rh+1][rw+2].value,
				(int) v0_nz_vec[chan][rh+1][rw+3].value ) ;
					__m256i v0_part2 = _mm256_setr_epi32((int) v0_nz_vec[chan][rh+2][rw+0].value,
				(int) v0_nz_vec[chan][rh+2][rw+1].value, (int) v0_nz_vec[chan][rh+2][rw+2].value,
				(int) v0_nz_vec[chan][rh+2][rw+3].value, (int) v0_nz_vec[chan][rh+3][rw+0].value,
				(int) v0_nz_vec[chan][rh+3][rw+1].value, (int) v0_nz_vec[chan][rh+3][rw+2].value,
				(int) v0_nz_vec[chan][rh+3][rw+3].value ) ;
					
					__m256i kernel_part1 = _mm256_setr_epi32(
						(int)kernel[0][0].value,(int)kernel[0][1].value,
						(int)kernel[0][2].value,(int)kernel[0][3].value,
						(int)kernel[1][0].value,(int)kernel[1][1].value,
						(int)kernel[1][2].value,(int)kernel[1][3].value);
					__m256i kernel_part2 = _mm256_setr_epi32(
						(int)kernel[2][0].value,(int)kernel[2][1].value,
						(int)kernel[2][2].value,(int)kernel[2][3].value,
						(int)kernel[3][0].value,(int)kernel[3][1].value,
						(int)kernel[3][2].value,(int)kernel[3][3].value);
					load_t_end = clock() ;
					load_t = load_t + ((double)(load_t_end - load_t_start)/CLOCKS_PER_SEC) ;
					__m256i v_result1 = _mm256_mullo_epi32(v0_part1,kernel_part1) ;
					__m256i v_result2 = _mm256_mullo_epi32(v0_part2,kernel_part2) ;
					int *result1 = (int*) &v_result1 ;
					int *result2 = (int*) &v_result2 ;
					
					for(int sum_i = 0 ; sum_i < 8 ;sum_i ++){
						channel_partial_sum = channel_partial_sum + result1[sum_i]+ result2[sum_i];
					}

					
					output[OC][rh][rw] += channel_partial_sum ;

				}
			}
		}
	}
//Print output 
	conv_end = clock() ;
	double conv_time = (double) (conv_end - conv_start)/CLOCKS_PER_SEC ;
	printf("CONV time : %lf LOAD TIME %lf \n", conv_time, load_t);
	/*
	for(int c = 0 ; c < 1 ; c ++){
		for(int x = 0 ; x < resultH ; x ++){
			for(int y = 0 ; y < resultW ; y ++ ) {
				printf("%d ", output[c][x][y]) ;
			}
			printf("\n");
		}
		printf("\n");
	}
	*/

}
int main (void){
	// Declare input
	clock_t start,end;
	start = clock() ;
	std::vector<std::vector<std::vector<nz_info> > > v0_nz_vec (CHANNEL,
			std::vector<std::vector<nz_info> > (HEIGHT)); // input image 
	std::vector<std::vector<nz_info> > kernel (4); // input kernel
	int ***output ;

	Generate_Input(v0_nz_vec, kernel) ;
	Convolution(v0_nz_vec, kernel,output) ;
	
	end = clock() ;
	double time_elasped = double(end - start)/ double(CLOCKS_PER_SEC);
	printf("Elasped time is %lf seconds\n", time_elasped);
	return 0 ;
}
