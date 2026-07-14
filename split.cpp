#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
using namespace std;

/// max records that fit in memory at once
int CHUNK_SIZE;

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

    ifstream in("input.txt");

    struct Record {
        string key;
        long long offset; 
    };

    const int RECORD_SIZE = 100;
    vector<Record> buffer;  
    buffer.reserve(CHUNK_SIZE); // reserves before to prevent repeasted allocaiton
    int runId = 0;

    // reads records until buffer is full 
    while (true) {
        streampos pos = in.tellg(); // gets pos 

        string line(RECORD_SIZE, '\0'); // read 100 bytes

        if (!in.read(&line[0], RECORD_SIZE)) {
            break;
        }

        Record record;
        record.offset = pos;
        record.key = line.substr(0, 10); // gets key 
        buffer.push_back(record);

        // buffer 
        if (buffer.size() == static_cast<size_t>(CHUNK_SIZE)) {
            sort(buffer.begin(), buffer.end(), [](const Record& a, const Record& b){
                return a.key < b.key;
            });

            ofstream out("run" + to_string(runId) + ".txt");

            // write buffer out 
            for (const Record& record : buffer) {
                out.write(record.key.data(), 10);
                out << record.offset << "\n";
            }

            buffer.clear();
            runId++;
            


        }
    }
    // clear remaining buffer 
    if (!buffer.empty()) {
        sort(buffer.begin(), buffer.end(), [](const Record& a, const Record& b){
                return a.key < b.key;
            });
        ofstream out("run" + to_string(runId) + ".txt");

        for (const Record& record : buffer) {
            out.write(record.key.data(), 10);
            out << record.offset << "\n";
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