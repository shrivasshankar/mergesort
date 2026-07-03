#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;


/// basic merge with runs
const int CHUNK_SIZE = 3;

int main() {
    ifstream in("input.txt");

    vector<int> buffer;
    int x;
    int runId = 0;

    while (in >> x) {
        buffer.push_back(x);
        // buffer 
        if (buffer.size() == CHUNK_SIZE) {
            sort(buffer.begin(), buffer.end());
            ofstream out("run" + to_string(runId) + ".txt");

            // write buffer out 
            for (int num : buffer) {
                out << num << "\n";
            }

            buffer.clear();
            runId++;
            


        }
    }
    // clear remaining buffer 
    if (!buffer.empty()) {
        sort(buffer.begin(), buffer.end());
        ofstream out("run" + to_string(runId) + ".txt");

        for (int num : buffer) {
            out << num << "\n";
        }
        buffer.clear();
        runId++;
    }





}