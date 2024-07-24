#include <cassert>
#include <vector>
#include <utility>
#include <queue>
#include <set>
#include <array>
#include <algorithm>
#include <limits.h>
#include <map>

using namespace std;
using vi = vector<int>;

int cores, p, n;
vi indeg, w, core_time, free_time;
set<int> free_nodes;

vector<array<int, 3>> res;
vector<array<int, 3>> act;

vector<vector<int>> adj;

int best_end = INT_MAX;

void solve() {
    if (free_nodes.empty()) {
        int end = 0;
        for (auto [s, e, c] : act) {
            end = max(end, e);
        }
        if (end < best_end) {
            res = act;
            best_end = end;
        }
        return;
    }
    for (auto i : free_nodes) {
        free_nodes.erase(i);
        for (int c = 0; c < cores; c++) {
            map<int, int> updated;
            int prev = core_time[c];
            int start = max(core_time[c], free_time[i]);
            int end = start + w[i];
            if (end > best_end) {
                // Euristica: non ricorro se piazzo un task oltre il miglior tempo che ho attualmente
                continue;
            }
            act[i] = {start, end, c};
            for (auto j : adj[i]) {
                updated[j] = free_time[j];
                free_time[j] = max(free_time[j], end);
                if (--indeg[j] == 0) {
                    free_nodes.insert(j);
                }
            }
            solve();
            for (auto j : adj[i]) {
                free_time[j] = updated[j];
                if (++indeg[j] == 1) {
                    free_nodes.insert(j);
                }
            }
            core_time[c] = prev;
        }
        free_nodes.insert(i);
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
    for (int i = 0; i < m; i++) {
        adj[a[i]].push_back(b[i]);
        indeg[b[i]]++;
    }
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) {
            free_nodes.insert(i);
        }
    }
    solve();
    return res;
}
