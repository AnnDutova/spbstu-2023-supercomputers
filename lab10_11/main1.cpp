#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    auto start_time = std::chrono::high_resolution_clock::now();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = std::atoi(argv[1]); 
    int *sendbuf = (int*) malloc(n * sizeof(int));
    int *recvbuf = (int*) malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        sendbuf[i] = rank * n + i;
    }

    MPI_Scan(sendbuf, recvbuf, n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Process %d: ", rank);
    for (int i = 0; i < n; i++) {
        printf("%d ", recvbuf[i]);
    }
    printf("\n");

    free(sendbuf);
    free(recvbuf);

    if (rank == 0) {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "Time: " << duration.count() << " mcs" << std::endl;
    } 
    MPI_Finalize();
    return 0;
}
