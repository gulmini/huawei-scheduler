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
vi indeg, w, core_time, free_time, free_nodes, max_depth;

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

    // claim: it is optimal to assing the task to the first free core if trying all toposorts
    int c = 0;
    for (int i = 1; i < cores; i++) {
        if (core_time[i] < core_time[c]) {
            c = i;
        }
    }

    for (int _i = 0; _i < dim; _i++) {
        int i = free_nodes[_i];

        int start = max(core_time[c], free_time[i]);
        int end = start + w[i];
        act[i] = {start, end, c};
        
        if (start + max_depth[i] >= best_end) continue; // do not recurr if actual max time is > best_end

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
            free_nodes.push_back(i);
        }
    }
    {
        max_depth.resize(n);
        vi toposort, ind = indeg;
        queue<int> q;
        for (auto i : free_nodes) q.push(i);
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            toposort.push_back(i);
            for (auto j : adj[i]) {
                if (--ind[j] == 0) {
                    q.push(j);
                }
            }
        }
        reverse(toposort.begin(), toposort.end());
        for (auto i : toposort) {
            for (auto j : adj[i]) {
                max_depth[i] = max(max_depth[j], max_depth[i]);
            }
            max_depth[i] += w[i];
        }
    }
    solve();
    return res;
}
