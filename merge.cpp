#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;
using namespace std;

const int RECORD_SIZE = 100;

// Stores a record being merged
// fileindex tracks which sorted run this index came from
struct Node {
    string key;
    string data; 
    int fileIndex;
};

// compare for a min heap so smallest is at the top; 
struct Compare{
     bool operator()(const Node& a, const Node& b) const {
        return a.key > b.key; 
    }
};

int main() {
    auto start = chrono::high_resolution_clock::now(); // start timer

    priority_queue<Node, vector<Node>, Compare> pq; 
    vector<string> runFiles;
    vector<ifstream> files;
 

    // find every run file generated during the split phase.

    for (const auto& entry : fs::directory_iterator(".")) {
        string filename = entry.path().filename().string();

        if (filename.rfind("run", 0) == 0 &&
            filename.substr(filename.size() - 4) == ".txt") {
            runFiles.push_back(filename);
        }
    }

    // sort so files ran files are in order 

    sort(runFiles.begin(), runFiles.end());

    // open each run file for sequential reading
    for (const string& filename : runFiles) {
        files.push_back(ifstream(filename, ios::binary));
    }

    // pushes the first value of each file into the priority queue
    for (int i = 0; i < files.size(); i++) {
        string record(RECORD_SIZE, '\0');
        if (files[i].read(&record[0], RECORD_SIZE)) {
            string key = record.substr(0,10);

            pq.push({key, record, i});
        }
    }
    // writes out
    ofstream out ("output.txt", ios::binary);
    // pops the smallest value and writes to output
    // then it inserts the record from the same run
    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();
        // uses offset to find real record 
        out.write(curr.data.data(), RECORD_SIZE);

        string nextRecord(RECORD_SIZE, '\0');

        // Load the next record from the same run file.
        if (files[curr.fileIndex].read(&nextRecord[0], RECORD_SIZE)) {
            string nextKey = nextRecord.substr(0,10);
            pq.push({nextKey, nextRecord, curr.fileIndex});
        }

    }

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;

    cout << "Merge time: "
        << elapsed.count()
        << " seconds\n";
    return 0;

}
