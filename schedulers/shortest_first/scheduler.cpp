#include <cassert>
#include <vector>
#include <utility>
#include <queue>
#include <set>
#include <array>
#include <algorithm>

using namespace std;

const char *scheduler_name = "shortest_first";

vector<array<int, 3>> schedule(int N, int M, int P, int Gamma, vector<int> Omega, vector<int> A, vector<int> B){
    vector<vector<int>> ad(N);
    vector<int> indeg(N), max_time(N);
    for(int i=0; i<M; i++){
        ad[A[i]].push_back(B[i]);
        indeg[B[i]]++;
    }

    priority_queue<array<int, 3>, vector<array<int, 3>>, greater<>> pq;
    for(int i=0; i<N; i++){
        if(indeg[i] == 0) pq.push({Omega[i], 0, i});
    }

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> cores;
    for(int i=0; i<Gamma; i++){
        cores.push({0, i});
    }

    vector<array<int, 4>> schedule;

    while(pq.size()){
        auto [len, time, v] = pq.top();
        pq.pop();

        assert(cores.size());
        auto [core_start_time, core] = cores.top();
        cores.pop();
        
        int start_time = max(core_start_time, time);
        int end_time = start_time + Omega[v];

        cores.push({end_time, core});
        schedule.push_back({v, start_time, end_time, core});

        for(int u: ad[v]){
            indeg[u]--;
            max_time[u] = max(max_time[u], end_time);

            if(indeg[u] == 0){
                pq.push({Omega[u], max_time[u], u});
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
