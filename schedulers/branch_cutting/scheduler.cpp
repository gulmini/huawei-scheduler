#include <cassert>
#include <vector>
#include <utility>
#include <queue>
#include <set>
#include <array>
#include <algorithm>
#include <limits.h>
#include <map>
#include <iostream>

using namespace std;
using vi = vector<int>;

int cores, p, n;
vi indeg, w, core_time, free_time, toposort, free_nodes;

vector<array<int, 3>> res;
vector<array<int, 3>> act;

vector<vector<int>> adj;

int best_end = INT_MAX;

void solve(int pos = 0) {
    if (pos == n) {
        int end = 0;
        for (auto [s, e, c] : act) {
            end = max(end, e);
        }
        if (end < best_end) {
            best_end = end;
            res = act;
        }
        return;
    }
    int dim = free_nodes.size();
    for (int _i = 0; _i < dim; _i++) {
        for (int c = 0; c < cores; c++) {
            int i = free_nodes[_i];

            int start = max(core_time[c], free_time[i]);
            int end = start + w[i];
            act[i] = {start, end, c};

            if (end > best_end) continue;

            int prev_time = core_time[c];
            core_time[c] = end;

            map<int, int> rev;
            for (auto j : adj[i]) {
                rev[j] = free_time[j];
                free_time[j] = max(free_time[j], end);
                if (--indeg[j] == 0) {
                    free_nodes.push_back(j);
                }
            }

            free_nodes[_i] = free_nodes.back();
            free_nodes.pop_back();

            solve(pos + 1);

            free_nodes.push_back(free_nodes[_i]);
            free_nodes[_i] = i;

            core_time[c] = prev_time;

            for (auto j : adj[i]) {
                free_time[j] = rev[j];
                if (++indeg[j] == 1) {
                    free_nodes.pop_back();
                }
            }
        }
    }
}

vector<array<int, 3>> schedule(int n, int m, int p, int y, vi w, vi a, vi b) {
    ::w = w;
    ::p = 0;
    ::n = n;
    cores = y;
    adj.resize(n);
    free_time.resize(n);
    core_time.resize(cores);
    act.resize(n);
    indeg.resize(n);
    toposort.resize(n);
    for (int i = 0; i < m; i++) {
        adj[a[i]].push_back(b[i]);
        indeg[b[i]]++;
    }
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) {
            free_nodes.push_back(i);
        }
    }
    solve();
    return res;
}
