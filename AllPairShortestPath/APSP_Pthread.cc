#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

using namespace std ;

const int INF = 100000000;
const int V = 10010;
void input(char* inFileName);
void output(char* outFileName);

void seq_FW();

int n, m;  // Number of vertices, edges
int Dist[V][V];
int threadnum ;
int realthreadnum ;
int each ;
int last ;

void threadcal (int startpos, int each , int k ) 
{
    for (int i = startpos; i <startpos+ each; i++) {
        for (int j = 0; j < n; j++) {
            if (Dist[i][j] > Dist[i][k] + Dist[k][j]) {
                Dist[i][j] = Dist[i][k] + Dist[k][j];
            }
        }
    }

}

int main(int argc, char* argv[]) {
    threadnum = atoi(argv[3]) ;
    input(argv[1]);
    if(threadnum >= n ) {
        realthreadnum = n ;
        each = 1 ;
        last = 1 ;
    }
    else {
        realthreadnum = threadnum ;
        each = n / realthreadnum ;
        last = n / realthreadnum + n % realthreadnum ;
    }
    thread T[realthreadnum] ;
    //    seq_FW();
    //cout << "ThreadNum = " << threadnum << "realthreadnum = "<< realthreadnum << endl ;
    for (int k = 0; k < n; k++) {
        for(int th = 0 ; th < realthreadnum ; th ++ ){
            if(th == realthreadnum - 1 ) {
                T[th] = thread(threadcal , th*each , last, k ); 
            }
            else{
                T[th] = thread(threadcal , th*each , each , k ); 
            }

        }
        for(int th = 0 ; th < realthreadnum ; th ++ ){
            T[th].join() ;
        }
    }
    /*
       for(int th = 0 ; th < realthreadnum ; th ++ ){
       T[th].join() ;
       }
       */
  //  cout<<"Done " << endl ;
    output(argv[2]);

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
