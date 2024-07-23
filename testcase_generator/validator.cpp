/*

*** TESTCASE FORMAT ***

n m p gamma
w_1 w_2 ... w_n
a_1 b_1
a_2 b_2
.   .
.   .
.   .
a_n b_n

*** GENERATOR PARAMETERS ***

n m p gamma lambda_min lambda_max L_min L_max

*/

#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <cmath>
#include <numeric>
#include <queue>
#include <set>

struct testcase {
    int n, m, p, gamma; 
    std::vector<int> w;
    std::vector<std::pair<int, int>> edges;

    void read_testcase(FILE *in) {
        assert(std::fscanf(in, "%d %d %d %d", &n, &m, &p, &gamma) == 4);
        w.resize(n);
        for (auto &x : w) {
            assert(std::fscanf(in, "%d", &x) == 1);
        }
        edges.resize(m);
        std::set<std::pair<int, int>> duplicates;
        for (auto &[a, b] : edges) {
            assert(std::fscanf(in, "%d %d", &a, &b) == 2);
            if (a < 0 || a >= n) {
                std::printf("Node not in range [0, n)\n");
                exit(1);
            }
            if (b < 0 || b >= n) {
                std::printf("Node not in range [0, n)\n");
                exit(1);
            }
            if (duplicates.count({a, b})) {
                std::printf("Duplicated edge!\n");
                exit(1);
            }
        }
        assert(std::fgetc(in) == '\n');
        if (std::fgetc(in) != EOF) {
            std::printf("Didn't reach EOF\n");
            exit(1);
        }
    }

    bool check_lambda(double lambda_min, double lambda_max) {
        for (auto x : w) {
            if (x < floor(p * lambda_min)) {
                return false;
            }
            if (x > floor(p * lambda_max)) {
                return false;
            }
        }
        return true;
    }

    bool check_L(double L_min, double L_max) {
        int sum_w = std::accumulate(w.begin(), w.end(), 0);
        if (sum_w < floor(p * L_min)) {
            return false;
        }
        if (sum_w > floor(p * L_max)) {
            return false;
        }
        return true;
    }

    bool is_dag() {
        std::vector<int> pos(n),out_deg(n);
        std::vector<std::vector<int>> adj(n);
        for (auto [a, b] : edges) {
            adj[b].push_back(a);
            out_deg[a]++;
        }
        int timer = n;
        std::queue<int> q;
        for (int i = 0; i < n; i++) {
            if (out_deg[i] == 0) {
                q.push(i);
            }
        }
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            pos[i] = timer--;
            for (auto j : adj[i]) {
                if (--out_deg[j] == 0) {
                    q.push(j);
                }
            }
        }
        if (timer != 0) {
            return false;
        }
        for (auto [a, b] : edges) {
            if (pos[a] >= pos[b]) {
                return false;
            }
        }
        return true;
    }
};

int main(int argc, char **argv) {
    if (argc < 2) {
        std::printf("Missing testcase name!\n");
        std::exit(1);
    }
    std::string tc_name = argv[1];

    FILE *tc_in = fopen(("../testcases/" + tc_name).c_str(), "r");
    if (!tc_in) {
        std::printf("Missing file in directory ../testcase/\n");
        std::exit(1);
    }

    testcase tc;
    tc.read_testcase(tc_in);

    FILE *param_in = fopen(("gen/" + tc_name).c_str(), "r");
    if (!param_in) {
        std::printf("Missing gen file in gen/\n");
        std::exit(1);
    }

    int n, m, p, gamma, seed;
    double L_min, L_max;
    double lambda_min, lambda_max;

    assert(std::fscanf(param_in, "%d %d %d %d %lf %lf %lf %lf %d", &n, &m, &p, &gamma, &lambda_min, &lambda_max, &L_min, &L_max, &seed) == 9);

    if (L_min < n * lambda_min) {
        std::printf("L_min is less than N * lambda_min\n");
        exit(1);
    }

    if (L_max > std::min(gamma, (int)std::floor(n * lambda_max))) {
        std::printf("L_min is greater than min (gamma, N * lambda_max)\n");
        exit(1);
    }

    if (tc.n != n) {
        std:printf("testcase N doesn't match generator N\n");
        exit(1);
    }
    if (tc.m != m) {
        std::printf("testcase M doesn't match generator M\n");
        exit(1);
    }
    if (tc.p != p) {
        std::printf("testcase P doesn't match generator P\n");
        exit(1);
    }
    if (tc.gamma != gamma) {
        std::printf("testcase GAMMA doesn't match generator GAMMA\n");
        exit(1);
    }
    
    if (!tc.check_lambda(lambda_min, lambda_max)) {
        std::printf("Values of lambda not in range [lambda_min, lambda_max]\n");
        exit(1);
    } 
    if (!tc.check_L(L_min, L_max)) {
        std::printf("Value of L not in range [L_min, L_max]\n");
        exit(1);
    }
    if (!tc.is_dag()) {
        std::printf("Not a DAG\n");
        exit(1);
    }
    return EXIT_SUCCESS;
}
