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

// node for records and tracking index
struct Node {
    string key;
    long long offset; 
    int fileIndex;
};

// compare for a min heap; 
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
    ifstream input("input.txt");

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

    // open each run file for reading
    for (string filename : runFiles) {
        files.push_back(ifstream(filename));
    }

    // pushes the first value of each file into the priority queue
    for (int i = 0; i < files.size(); i++) {
        string key;
        long long offset; 
        string line;
        if (getline(files[i], line)) {
            key = line.substr(0, 10);
            offset = stoll(line.substr(10));

            pq.push({key, offset, i});
        }
    }
    // writes out
    ofstream out ("output.txt");
    // pops the smallest value 
    // from the priority queue and writes it to the output file, then pushes the next value from the same file into the priority queue
    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();
        // uses offset to find real record 
        input.seekg(curr.offset);

        string record(100, '\0');
        input.read(&record[0], 100);

        out.write(record.data(), 100);

        string nextKey;
        long long nextOffset;
        string line;        

        if (getline(files[curr.fileIndex], line)) {
            nextKey = line.substr(0, 10);
            nextOffset = stoll(line.substr(10));
            pq.push({nextKey, nextOffset, curr.fileIndex});
        }

    }

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;

    cout << "Merge time: "
        << elapsed.count()
        << " seconds\n";
    return 0;

}
