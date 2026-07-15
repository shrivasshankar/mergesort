#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
using namespace std;

// max records in memory
int CHUNK_SIZE;
const int RECORD_SIZE = 100;

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

    struct Record {
        string key;
        string data;
    };

    vector<Record> buffer;  
    buffer.reserve(CHUNK_SIZE); // reserves before to prevent repeatd allocaiton
    int runId = 0;

    // reads records until buffer/chunk is full 
    while (true) {
        string line(RECORD_SIZE, '\0');

        if (!in.read(&line[0], RECORD_SIZE)) {
            break;
        }

        Record record;
        record.key = line.substr(0, 10); // gets first 10 bytes as key 
        // Store the complete record so merging does not require
        // random access back to the original input file. 
        // this caused many problems when i implemented wiscsorts's pointer based merge sort
        // which cause splitting to be quick but merging to be absurrdly long on 50gb
        record.data = line; 
        buffer.push_back(record);

        // when memory is full sort the chunk and write a sorted run
        if (buffer.size() == static_cast<size_t>(CHUNK_SIZE)) {
            sort(buffer.begin(), buffer.end(), [](const Record& a, const Record& b){
                return a.key < b.key;
            });

            ofstream out("run" + to_string(runId) + ".txt", ios::binary);

            // write buffer out 
            for (const Record& record : buffer) {
                out.write(record.data.data(), RECORD_SIZE);
            }

            buffer.clear();
            runId++;
            


        }
    }
    // write any remaining records that did not fill a complete chunk 
    if (!buffer.empty()) {
        sort(buffer.begin(), buffer.end(), [](const Record& a, const Record& b){
                return a.key < b.key;
            });
        ofstream out("run" + to_string(runId) + ".txt", ios::binary);

        for (const Record& record : buffer) {
            out.write(record.data.data(), RECORD_SIZE);
        }
        buffer.clear();
        runId++;
    }
    auto end = chrono::high_resolution_clock::now(); // end timer
    chrono::duration<double> elapsed = end - start;
    cout << "Split time: "
        << elapsed.count()
        << " seconds\n";





}