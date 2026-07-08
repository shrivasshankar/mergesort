#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;



/// basic merge with runs
const int CHUNK_SIZE = 10000;

int main() {
    ifstream in("input.txt");

    vector<string> buffer;
    string record;
    int runId = 0;

    while (getline(in, record)) {
        buffer.push_back(record);
        // buffer 
        if (buffer.size() == CHUNK_SIZE) {
            sort(buffer.begin(), buffer.end());
            ofstream out("run" + to_string(runId) + ".txt");

            // write buffer out 
            for (const string& record : buffer) {
                out << record << "\n";
            }

            buffer.clear();
            runId++;
            


        }
    }
    // clear remaining buffer 
    if (!buffer.empty()) {
        sort(buffer.begin(), buffer.end());
        ofstream out("run" + to_string(runId) + ".txt");

        for (const string& record : buffer) {
            out << record << "\n";
        }
        buffer.clear();
        runId++;
    }





}