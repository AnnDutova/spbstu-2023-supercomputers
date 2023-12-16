#include <iostream>
#include <math.h>
#include <omp.h>

#define N 4

int main(int argc, char *argv[]) {
    double x[N],y[N];
    x[0] = 0;
    # pragma omp parallel for nowait
    for (int i=1; i<N; i++) {
        x[i] = x[i-1]*log(i);
        y[i] = x[i] /y[i-1];
    }
    y[0]=x[N-1]; 
    return 0;
}
