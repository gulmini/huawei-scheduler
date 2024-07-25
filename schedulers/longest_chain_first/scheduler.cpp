#include <cassert>
#include <vector>
#include <utility>
#include <queue>
#include <set>
#include <array>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

vector<array<int, 3>> schedule(int N, int M, int P, int Gamma, vector<int> Omega, vector<int> A, vector<int> B){
    vector<vector<int>> ad(N);
    vector<int> indeg(N), max_time(N);
    for(int i=0; i<M; i++){
        ad[A[i]].push_back(B[i]);
        indeg[B[i]]++;
    }

    vector<int> chain(N), vis(N), sum(N);
    function<void(int)> calc_chain = [&](int v){
        chain[v] = Omega[v];
        vis[v] = 1;
        for(int u: ad[v]){
            if(!vis[u]) calc_chain(u);
            chain[v] = max(chain[v], chain[u] + Omega[v]);
        }
    };
    for(int i=0; i<N; i++) if(!vis[i]) calc_chain(i);
    
    function<void(int, int)> calc_sum = [&](int v, int p){
        sum[p] += Omega[v];
        vis[v] = 1;
        for(int u: ad[v]){
            if(!vis[u]) calc_sum(u, p);
        }
    };
    for(int i=0; i<N; i++){
        sum[i] = 0;
        vis.assign(N, 0);
        calc_sum(i, i);
    }

    vector<int> bound(N);
    for(int i=0; i<N; i++){
        bound[i] = max(chain[i], (sum[i]-1)/Gamma + 1);
        // cerr << i << " " << bound[i] << " " << sum[i] << endl;
    }

    priority_queue<array<int, 3>> pq;
    for(int i=0; i<N; i++){
        if(indeg[i] == 0) pq.push({bound[i], 0, i});
    }

    set<pair<int,int>> cores;
    for(int i=0; i<Gamma; i++){
        cores.insert({0, i});
    }

    vector<array<int, 4>> schedule;

    while(pq.size()){
        auto [tmp, time, v] = pq.top();
        pq.pop();

        assert(cores.size());
        auto it = cores.upper_bound({time, 0});
        if(it != cores.begin()) it--;
        auto [core_start_time, core] = *it;
        
        int start_time = max(core_start_time, time);
        int end_time = start_time + Omega[v];

        cores.erase(it);
        cores.insert({end_time, core});
        schedule.push_back({v, start_time, end_time, core});

        for(int u: ad[v]){
            indeg[u]--;
            max_time[u] = max(max_time[u], end_time);

            if(indeg[u] == 0){
                pq.push({bound[u], max_time[u], u});
            }
        }
    }

    sort(schedule.begin(), schedule.end());
    vector<array<int, 3>> ans;
    for(auto [task, s, e, c]: schedule){
        ans.push_back({s, e, c});
    }
    return ans;
}
