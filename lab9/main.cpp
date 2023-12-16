#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <mpi.h>

using namespace std;

string readFileData(char* filename) {
    MPI_File file;
    MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);

    MPI_Offset filesize;
    MPI_File_get_size(file, &filesize);
    
    string data;
    data.resize(filesize);
    MPI_File_read_at_all(file, 0, const_cast<char*>(data.c_str()), filesize, MPI_CHAR, MPI_STATUS_IGNORE);
    MPI_File_close(&file);

    return data;
}


vector<int> convertToIntVector(string input, int start, int end) {
    istringstream iss(input);
    vector<int> data;
    int value;
    while (iss >> value) {
        data.push_back(value);
    }
    return data;
}

vector<int> getChunk(vector<int> data, int start, int end) {
    vector<int> chunk;
    chunk.assign(data.begin() + start, data.begin() + end);
    return chunk;
}

long long calculate(vector<int> data) {
    long long result = 1;
     for (const auto& value : data) {
        result *= value;
    }
    return result;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    ofstream outFile(argv[2], ios::app);
    if (!outFile.is_open()) {
        cerr << "Can't open file" << endl;
        return 1; 
    }

    string fileStringData = readFileData(argv[1]);
    vector<int> data = convertToIntVector(fileStringData, rank, rank + 5);
    for (int iter = 1; iter <= data.size(); iter++) {
        int chunkSize = (iter / size);
        int start = rank * chunkSize;
        int end = (rank + 1) * chunkSize;

        if (rank == size - 1) {
            end += (iter % size);
        }

        vector<int> chunk = getChunk(data, start, end);

        long long local_result = calculate(chunk);

        vector<int> allResults(size);
        MPI_Gather(&local_result, 1, MPI_INT, allResults.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            /*for (int i = 0; i < iter; i++) {
                cout << data[i] << " ";
            }
            cout << endl;*/


            if (iter >= size) {
                long long result = calculate(allResults);
                // cout << "C=" << iter << " Proc " << rank << " result: " << result << endl;
                outFile << "C=" << iter << " result: " << result << endl;   
            } else {
                // cout << "C=" << iter << " Proc " << rank << " result: " << allResults[size-1] << endl; 
                outFile << "C=" << iter << " result: " << allResults[size-1]  << endl;

            }
        }
    }
    outFile.close();

    
    MPI_Finalize();
    return 0;
}