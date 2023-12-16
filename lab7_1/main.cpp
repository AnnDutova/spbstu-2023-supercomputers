#include <iostream>
#include <math.h>
#include <omp.h>

#define N 4

int main(int argc, char *argv[]) {
    double x[N], y[N];
    x[0] = 0;

    #pragma omp parallel 
    {
        #pragma omp for
        for (int i = 1; i < N; i++) {
            x[i] = x[i - 1] * log(i);
        }
        
        y[0] = x[N - 1];
        
        #pragma omp for
        for (int i = 1; i < N; i++) {
            y[i] = x[i] / y[i - 1];
        }
    }

    return 0;
}