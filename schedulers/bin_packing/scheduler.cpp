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

const char *scheduler_name = "bin_packing";

int n, cores, opt_bound; 
vi w, assigned_core, indeg, load, sorted_tasks;
vector<vi> adj;
vector<array<int, 3>> best;
int best_end = INT_MAX;

void solve(int _i, int first_free = 1) {
    if (best_end == opt_bound) return;
    int i = sorted_tasks[_i];
    if (_i == n) {
        vi temp_indeg = indeg;
        int ans = 0;
        vi core_time(cores);
        vi free_time(n);
        vector<array<int, 3>> act(n);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        for (int j = 0; j < n; j++) {
            if (temp_indeg[j] == 0) {
                pq.push({0, j});
            }
        }
        while (!pq.empty()) {
            auto [t, j] = pq.top();
            pq.pop();
            if (t < core_time[assigned_core[j]]) {
                pq.push({core_time[assigned_core[j]], j});
                continue;
            }
            act[j] = {t, t + w[j], assigned_core[j]};
            core_time[assigned_core[j]] = t + w[j];
            ans = max(ans, t + w[j]);
            for (int k : adj[j]) {
                free_time[k] = max(free_time[k], t + w[j]);
                if (--temp_indeg[k] == 0) {
                    pq.push({max(free_time[k], core_time[assigned_core[k]]), k});
                }
            }
        }
        if (ans < best_end) {
            cerr << ans << endl;
            best_end = ans;
            best = act;
        }
        return;
    }
    vi sorted;
    for (int c = 0; c < min(cores, first_free + 1); c++) {
        sorted.push_back(c);
    }
    sort(sorted.begin(), sorted.end(), [&](int x, int y){
        return load[x] < load[y];
    });
    for (auto c : sorted) {
        load[c] += w[i];
        if (load[c] >= best_end) {
            load[c] -= w[i];
            continue;
        }
        assigned_core[i] = c;
        solve(_i + 1, max(first_free, c + 1));
        load[c] -= w[i];
    }
}

vector<array<int, 3>> schedule(int n, int m, int p, int y, vi w, vi a, vi b) {
    ::n = n;
    ::cores = y;
    ::w = w;
    adj.resize(n);
    assigned_core.resize(n);
    indeg.resize(n);
    load.resize(cores);
    sorted_tasks.resize(n);
    iota(sorted_tasks.begin(), sorted_tasks.end(), 0);
    srand(420);
    random_shuffle(sorted_tasks.begin(), sorted_tasks.end());
    for (int i = 0; i < m; i++) {
        adj[a[i]].push_back(b[i]);
        indeg[b[i]]++;
    }

    {   
        vi max_depth(n);
        vi toposort, ind = indeg;
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                q.push(i);
            }
        }
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
        opt_bound = *max_element(max_depth.begin(), max_depth.end());
        opt_bound = max(opt_bound, (accumulate(w.begin(), w.end(), 0) + cores - 1) / cores);
    }


    solve(0);

    return best;
}
