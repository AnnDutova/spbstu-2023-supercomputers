#include <mpi.h>
#include <chrono>
#include <vector>

void MPI_Scan_Sum(const int* sendbuf, int* recvbuf, int count, int root, MPI_Comm comm) {
    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    if (rank == root) {
        std::copy(sendbuf, sendbuf + count, recvbuf);
    } else {
        MPI_Recv(recvbuf, count, MPI_INT, rank - 1, 0, comm, MPI_STATUS_IGNORE);
        for (int i = 0; i < count; ++i) {
            recvbuf[i] += sendbuf[i];
        }
    }

    if (rank != size - 1) {
        MPI_Send(recvbuf, count, MPI_INT, rank + 1, 0, comm);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    auto start_time = std::chrono::high_resolution_clock::now();
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = std::atoi(argv[1]); 
    int *sendbuf = (int*) malloc(n * sizeof(int));
    int *recvbuf = (int*) malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        sendbuf[i] = rank * n + i;
    }

    MPI_Scan_Sum(sendbuf, recvbuf, n, 0, MPI_COMM_WORLD);

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
