#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <set>
#include "Pagerank.h"
#include <cstring>

using namespace std;
using namespace chrono;

void usage() {
    cerr << "pagerank [-a alpha ] [-c convergence] [-b blocknums]"
        << "[-m max_iterations] <graph_file>" << endl
        << " -a alpha" << endl
        << "    default is 0.85 " << endl
        << " -c convergence" << endl
        << "    the convergence criterion default is 1e-5" << endl
        << " -m max_iterations" << endl
        << "    maximum number of iterations to perform" << endl
        << " -b blocknums" << endl
        << "    the number of blocknums, for some reason, the number shouldn't be larger than 100" << endl
        << " -s show the result" << endl
        << "    integer default all" << endl
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
            long double tmpc = atoi(argv[i + 1]);
            s.setShowNum(tmpc);
            i++;
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

    return 0;
}