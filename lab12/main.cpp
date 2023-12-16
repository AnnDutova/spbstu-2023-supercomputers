#include <mpi.h>
#include <iostream>
#include <vector>

#define N 8

using namespace std;

bool isSafe(int board[N], int row, int col) {
    for (int i = 0; i < col; i++)
        if (board[i] == row || abs(board[i] - row) == abs(i - col))
            return false;
    return true;
}

void printSolution(int board[N]) {
    static int k = 1;
    cout << "Solution " << k++ << ":\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << ((board[j] == i) ? "Q " : "- ");
        cout << "\n";
    }
    cout << "\n";
}

void solve(int board[N], int col, int &count) {
    if (col == N) {
        count++;
        printSolution(board);
        return;
    }
    for (int i = 0; i < N; i++) {
        if (isSafe(board, i, col)) {
            board[col] = i;
            solve(board, col + 1, count);
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = 0;
    int board[N];

    for (int i = rank; i < N; i += size) {
        board[0] = i;
        cout << "Process " << rank << " is working on solution starting with " << i << "\n";
        solve(board, 1, count);
    }

    int total;
    MPI_Reduce(&count, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        cout << "Total solutions: " << total << endl;
    
    MPI_Finalize();
    return 0;
}
