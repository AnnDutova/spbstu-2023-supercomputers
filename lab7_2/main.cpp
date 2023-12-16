#include <iostream>
#include <fstream>
#include <omp.h>

using namespace std;

const int MIN_DIVIDER = 2;
const int MAX_DIVIDER = 10;

int readFile(string filename) {
    int number;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Can't open file" << endl;
        return 1;
    }
    
    inputFile >> number;

    inputFile.close();
    
    return number;
}

int main(int argc, char *argv[]) {
    
    int number = readFile(argv[1]);

#pragma omp parallel for
    for (int divisor = MIN_DIVIDER; divisor <= MAX_DIVIDER; ++divisor) {
        int threadNum = omp_get_thread_num();
        if (number % divisor == 0) {
#pragma omp critical
            {
                cout << "Thread " << threadNum << ": " << number << " divisible by " << divisor << endl;
            }
        } else {
#pragma omp critical
            {
                cout << "Thread " << threadNum << ": " << number << " not divisible by " << divisor << endl;
            }
        }
    }

    return 0;
}