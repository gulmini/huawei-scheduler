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
#include <functional>
#include <numeric>

using namespace std;
using vi = vector<int>;

const char *scheduler_name = "claim";

int cores, p, n, free_space, to_be_placed;
vi indeg, w, core_time, free_time, free_nodes, max_depth, bound, sub_sum;

int c1 = 0, c2 = 0, c3 = 0;

vector<array<int, 3>> res;
vector<array<int, 3>> act;

vector<vector<int>> adj;

int best_end = INT_MAX;
int timer_start, opt_bound;

void solve(int pos = 0) {
    if (best_end == opt_bound) return;
    if (pos == n) {
        int end = 0;
        for (auto [s, e, c] : act) {
            end = max(end, e);
        }
        if (end < best_end) {
            cerr << end << endl;
            free_space -= cores * (best_end - end);
            best_end = end;
            res = act;
        }
        return;
    }
    int dim = free_nodes.size();
    vi order(dim);
    for (int i = 0; i < dim; i++) {
        order[i] = i;
    }
    sort(order.begin(), order.end(), [&](int i, int j){
        return max_depth[free_nodes[i]] > max_depth[free_nodes[j]];
    });

    int c_min = 0;
    vector<pair<int, int>> sorted_cores;
    for (int i = 0; i < cores; i++) {
        sorted_cores.push_back({core_time[i], i});
        if (core_time[i] < core_time[c_min]) {
            c_min = i;
        }
    }
    sort(sorted_cores.begin(), sorted_cores.end());
    vector<int> cores_to_try = {c_min};

    for (auto _i : order) {
        int i = free_nodes[_i];
        auto it = upper_bound(sorted_cores.begin(), sorted_cores.end(), make_pair(free_time[i] + 1, -1));
        if (it != sorted_cores.begin()) it--;
        cores_to_try[0] = it->second;

        for (auto c : cores_to_try) {
            int start = max(core_time[c], free_time[i]);
            int end = start + w[i];
            act[i] = {start, end, c};
            
            if (start + max_depth[i] >= best_end) {
                c1++;
                continue; 
            }
            if ((best_end - end) * cores < sub_sum[i]) {
                c2++;
                continue;
            }
            if (free_space - cores < to_be_placed) {
                c3++;
                continue;
            }

            int prev_time = core_time[c];
            core_time[c] = end;

            free_space -= end - prev_time;

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
            to_be_placed -= w[i];

            solve(pos + 1);

            to_be_placed += w[i];
            free_nodes.push_back(free_nodes[_i]);
            free_nodes[_i] = i;

            free_space += end - prev_time;
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

void james_bound(int n) {
    vi chain(n), vis(n), sum(n);
    sub_sum.resize(n);
    
    function<void(int, int)> calc_sum = [&](int v, int p){
        sum[p] += w[v];
        vis[v] = 1;
        for (int u : adj[v]) {
            if (!vis[u]) calc_sum(u, p);
        }
    };
    for (int i = 0; i < n; i++) {
        sum[i] = 0;
        vis.assign(n, 0);
        calc_sum(i, i);
        sub_sum[i] = sum[i] - w[i];
    }

    opt_bound = *max_element(max_depth.begin(), max_depth.end());
    opt_bound = max(opt_bound, (accumulate(w.begin(), w.end(), 0) + cores - 1) / cores);
}

vector<array<int, 3>> schedule(int n, int m, int p, int y, vi w, vi a, vi b) {
    timer_start = clock() * 1000;
    ::w = w;
    ::p = 0;
    ::n = n;
    cores = y;
    adj.resize(n);
    free_time.resize(n);
    core_time.resize(cores);
    act.resize(n);
    indeg.resize(n);
    to_be_placed = accumulate(w.begin(), w.end(), 0);
    best_end = to_be_placed + 10;
    free_space = cores * best_end;
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
    james_bound(n);
    solve();
    return res;
}
