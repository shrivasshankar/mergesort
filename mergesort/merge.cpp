#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>


using namespace std;


struct Node {
    int value;
    int fileIndex;

};

struct Compare{
     bool operator()(Node a, Node b) {
        return a.value > b.value; // compare for a min heap; 
    }
};

int main() {
    priority_queue<Node, vector<Node>, Compare> pq; 
    vector<ifstream> files;

    files.push_back(ifstream("run0.txt"));
    files.push_back(ifstream("run1.txt"));
    files.push_back(ifstream("run2.txt"));


    for (int i = 0; i < files.size(); i++) {
        int value;
        if (files[i] >> value) {
            pq.push({value, i});
        }
    }

    ofstream out ("output.txt");

    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();
        out << curr.value << "\n";

        int nextVal;
        if (files[curr.fileIndex] >> nextVal) {
            pq.push({nextVal, curr.fileIndex});
        }

    }
    return 0;

}
