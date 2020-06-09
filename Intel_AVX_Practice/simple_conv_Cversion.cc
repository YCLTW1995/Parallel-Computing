#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


#define CHANNEL 3
#define HEIGHT 2400
#define WIDTH 1600
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
	conv_start = clock() ;
	for(int OC = 0 ; OC < output_channel ; OC++){
		for(int chan = 0 ; chan < CHANNEL ; chan ++){
			for(int rh = 0 ; rh < resultH ; rh ++){
				for(int rw = 0 ; rw < resultW ; rw ++){
					int channel_partial_sum = 0 ;
					for(int k = 0 ; k < 16 ; k ++){
						channel_partial_sum += v0_nz_vec[chan][rh+k/4][rw+k%4].value*kernel[k/4][k%4].value ;
					}
					output[OC][rh][rw] += channel_partial_sum ;
				}
			}
		}
	}
//Print output 
	conv_end = clock() ;
	double conv_time = (double)(conv_end - conv_start) / CLOCKS_PER_SEC ;
	printf("CONV time : %lf\n", conv_time) ;
// Print result image 
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
