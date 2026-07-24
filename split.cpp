#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <cstdint> 
using namespace std;

// max records in memory
int CHUNK_SIZE;
const int RECORD_SIZE = 100;

struct KeyIdx {
        char key[10];
        uint32_t idx;   // record's slot within the chunk buffer
};

int main(int argc, char* argv[]) {
    auto start = chrono::high_resolution_clock::now(); // start timer
    if (argc != 2) {
        cout << "Usage: ./split_program <chunk_size>\n";
        return 1;
    }

    CHUNK_SIZE = stoi(argv[1]);

    if (CHUNK_SIZE <= 0) {
        cout << "Chunk size must be positive\n";
        return 1;
    }

    ifstream in("input.txt", ios::binary);

    vector<char> buf(static_cast<size_t>(CHUNK_SIZE) * RECORD_SIZE); // full records
    vector<char> out(static_cast<size_t>(CHUNK_SIZE) * RECORD_SIZE); // reordered
    vector<KeyIdx> keys;
    keys.reserve(CHUNK_SIZE);
    int runId = 0;

    // reads records until buffer/chunk is full 
    while (true) {
        // fill one chunk of full records
        size_t n = 0;
        while (n < static_cast<size_t>(CHUNK_SIZE) &&
               in.read(&buf[n * RECORD_SIZE], RECORD_SIZE)) {
            n++;
        }
        if (n == 0) break;

        // build small key+index array and sort ONLY that
        keys.resize(n);
        for (size_t i = 0; i < n; i++) {
            memcpy(keys[i].key, &buf[i * RECORD_SIZE], 10);
            keys[i].idx = static_cast<uint32_t>(i);
        }
        sort(keys.begin(), keys.end(),
             [](const KeyIdx& a, const KeyIdx& b){
                 return memcmp(a.key, b.key, 10) < 0;
             });

        // reorder the buffer ONE time: gather full records in sorted order
        for (size_t i = 0; i < n; i++)
            memcpy(&out[i * RECORD_SIZE], &buf[keys[i].idx * RECORD_SIZE], RECORD_SIZE);

        // one sequential write of full, sorted records
        ofstream run("run" + to_string(runId) + ".dat", ios::binary);
        run.write(out.data(), static_cast<streamsize>(n) * RECORD_SIZE);
        runId++;
    }

    auto end = chrono::high_resolution_clock::now(); // end timer
    chrono::duration<double> elapsed = end - start;
    cout << "Split time: "
        << elapsed.count()
        << " seconds\n";





}