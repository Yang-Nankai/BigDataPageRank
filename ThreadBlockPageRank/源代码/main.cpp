#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <set>
#include "Pagerank.h"
#include "mem.h"
#include <cstring>

using namespace std;
using namespace chrono;

void usage() {
    cerr << "pagerank [-a alpha ] [-c convergence] [-b blocknums]"
        << "[-m max_iterations] [-s show_top_nums] [-t thread_nums]<graph_file>" << endl
        << " -a alpha" << endl
        << "    default is 0.85 " << endl
        << " -c convergence" << endl
        << "    the convergence criterion default is 1e-5" << endl
        << " -m max_iterations" << endl
        << "    maximum number of iterations to perform" << endl
        << " -b blocknums" << endl
        << "    the number of blocknums" << endl
        << " -s show the result" << endl
        << "    integer default 0, if less than 0 will show all results" << endl
        << " -t threadnums" << endl
        << "    the number of thread, default is 1, and not more than blocknums"
        << " the output file named out.txt" << endl;
}

int main(int argc, char** argv) {
    string input = "";
    PageRank s;
    s.setMaxIteration(1000);
    s.setBeta(0.85);
    s.setBlockNum(4);
    for (size_t i = 1; i < argc; i++) {
        if (string(argv[i]) == "-m") {
            int tmpt = stoi(string(argv[i + 1]));
            if (tmpt <= 0) {
                usage();
                exit(1);
            }
            s.setMaxIteration(tmpt);
            i++;
        }
        else if (string(argv[i]) == "-a") {
            long double tmpa = atof(argv[i + 1]);
            if (tmpa > 1 || tmpa < 0) {
                usage();
                exit(1);
            }
            s.setBeta(tmpa);
            i++;
        }
        else if (string(argv[i]) == "-b") {
            int tmpt = stoi(string(argv[i + 1]));
            if (tmpt <= 0 || tmpt >= 100) {
                usage();
                exit(1);
            }
            s.setBlockNum(tmpt);
            i++;
        }
        else if (string(argv[i]) == "-c") {
            long double tmpc = atof(argv[i + 1]);
            s.setConvergence(tmpc);
            i++;
        }
        else if (string(argv[i]) == "-s") {
            int tmpc = stoi(argv[i + 1]);
            s.setShowNum(tmpc);
            i++;
        }
        else if (string(argv[i]) == "-t") {
            int tmpt = stoi(argv[i + 1]);
            if (tmpt < 1 || tmpt > s.getBlockNum()) {
                usage();
                exit(1);
            }
            s.setThreadNum(tmpt);
            i++;
        }
        else if (string(argv[i]) == "-h") {
            usage();
            exit(1);
        }
        else if (i == argc - 1) {
            input = argv[i];
        }
        else {
            usage();
            exit(1);
        }
    }
    if (input == "") {
        usage();
        exit(1);
    }
    s.getItemRange(input);
    auto start = steady_clock::now();
    s.readFile(input);
    s.print();
    s.pageRank();
    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "use "
        << double(duration.count()) * microseconds::period::num / microseconds::period::den
        << " s" << endl;
    double currentSize = (double)getCurrentRSS() / 1024 / 1024;
    cout << "Calculate memory cost: " << currentSize << " MB" << endl;

    return 0;
}