#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;


struct Node {
    string record;
    int fileIndex;

};

struct Compare{
     bool operator()(Node a, Node b) {
        return a.record > b.record; // compare for a min heap; 
    }
};

int main() {
    priority_queue<Node, vector<Node>, Compare> pq; 
    vector<string> runFiles;
    vector<ifstream> files;

    for (const auto& entry : fs::directory_iterator(".")) {
        string filename = entry.path().filename().string();

        if (filename.rfind("run", 0) == 0 &&
            filename.substr(filename.size() - 4) == ".txt") {
            runFiles.push_back(filename);
        }
    }

    sort(runFiles.begin(), runFiles.end());

    for (string filename : runFiles) {
        files.push_back(ifstream(filename));
    }

    // pushes the first value of each file into the priority queue
    for (int i = 0; i < files.size(); i++) {
        string record;
        if (getline(files[i], record)) {
            pq.push({record, i});
        }
    }
    // writes out
    ofstream out ("output.txt");
    // pops the smallest value 
    // from the priority queue and writes it to the output file, then pushes the next value from the same file into the priority queue
    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();
        out << curr.record << "\n";

        string nextRecord;
        if (getline(files[curr.fileIndex], nextRecord)) {
            pq.push({nextRecord, curr.fileIndex});
        }

    }
    return 0;

}
