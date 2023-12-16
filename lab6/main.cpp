#include <iostream>
#include <omp.h>
#include <vector>
#include <string>

using namespace std;

const string KEY_WORD = "key_word";
const int COUNT_OF_THREADS = 5;

bool containsKeyword(const vector<string> &words, const string &keyword) {
    for (const string& word : words) {
        if (word == keyword) {
            return true;
        }
    }
    return false;
}

int main() {
    vector<string> phrase = {
        "word", "word", "word", "word", "word", "word",
        "word", "word", "word", "word", "word", "word",
        "word", "word", "word", "word", "word", "word",
        "word", "word", "word", "word", "word", "word",
        "word", "word", "key_word", "word", "word", "word"
    };
    int wordsPerThread = phrase.size() / COUNT_OF_THREADS;
    bool keywordFound = false;

#pragma omp parallel num_threads(COUNT_OF_THREADS)
    {
        int threadNum = omp_get_thread_num();
        int startIdx = threadNum * wordsPerThread;
        int endIdx = (threadNum == COUNT_OF_THREADS - 1) ? phrase.size() : (threadNum + 1) * wordsPerThread;

#pragma omp critical
        {
            cout << "Thread " << threadNum << " processes words " << startIdx + 1 << " по " << endIdx << endl;
        }

        for (int i = startIdx; i < endIdx; ++i) {
            if (phrase[i] == KEY_WORD) {
#pragma omp critical
                {
                    keywordFound = true;
                    cout << "Thread " << threadNum << " found key_word" << endl;
                }
#pragma omp cancel parallel
            }
        }
    }

    if (keywordFound) {
        cout << "The door is open" << endl;
    } else {
        cout << "Invalid passphrase" << endl;
    }

    return 0;
}
