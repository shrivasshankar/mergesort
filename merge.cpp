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

// Stores a key pointer pair being merged
// fileindex tracks which sorted run this index came from
struct Node {
    string key;
    long long pointer;
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
    ifstream values("values.dat", ios::binary);
 

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

        string line;

        if (getline(files[i], line)) {

            string key = line.substr(0, 10);
            long long pointer = stoll(line.substr(11));

            pq.push({key, pointer, i});
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
        string record(RECORD_SIZE, '\0');

        values.seekg(curr.pointer);
        values.read(&record[0], RECORD_SIZE);

        out.write(record.data(), RECORD_SIZE);

        // Load the next key-pointer pair from the same run file.
        string line;

        if (getline(files[curr.fileIndex], line)) {

            string nextKey = line.substr(0, 10);
            long long nextPointer = stoll(line.substr(11));

            pq.push({nextKey, nextPointer, curr.fileIndex});
        }

    }

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;

    cout << "Merge time: "
        << elapsed.count()
        << " seconds\n";
    return 0;

}
