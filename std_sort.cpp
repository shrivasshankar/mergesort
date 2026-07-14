#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>

using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();

    ifstream in("input.txt");

    const int RECORD_SIZE = 100;

    vector<string> records;

    // read entire file into RAM
    while (true) {
        string record(RECORD_SIZE, '\0');

        if (!in.read(&record[0], RECORD_SIZE)) {
            break;
        }

        records.push_back(record);
    }

    cout << "Records loaded: " << records.size() << "\n";

    // sort by key (first 10 bytes)
    sort(records.begin(), records.end(), [](const string& a, const string& b) {
        return a.substr(0, 10) < b.substr(0, 10);
    });

    // write output
    ofstream out("std_output.txt");

    for (const string& record : records) {
        out.write(record.data(), RECORD_SIZE);
    }

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;

    cout << "std::sort time: "
         << elapsed.count()
         << " seconds\n";

    return 0;
}