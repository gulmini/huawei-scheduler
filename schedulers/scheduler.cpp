#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <queue>
#include <set>
#include <array>
#include <algorithm>

using namespace std;

int main(int argc, char **argv){
    assert(argc >= 2);
    string testcase = argv[1];
    string testcase_path = "../testcases/" + testcase;
    string output_path = "../output/" + testcase;

    ifstream in(testcase_path);
    ofstream out(output_path);

    int n, m, p, gamma;
    in >> n >> m >> p >> gamma;

    vector<int> omega(n);
    for(int &x: omega) in >> x;

    vector<vector<int>> ad(n);
    vector<int> indeg(n), max_time(n);
    for(int i=0; i<m; i++){
        int a, b; in >> a >> b;
        ad[a].push_back(b);
        indeg[b]++;
    }

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    for(int i=0; i<n; i++){
        if(indeg[i] == 0) pq.push({0, i});
    }

    set<pair<int,int>> cores;
    for(int i=0; i<gamma; i++){
        cores.insert({0, i});
    }

    vector<array<int, 4>> schedule;

    while(pq.size()){
        auto [time, v] = pq.top();
        pq.pop();

        auto core_it = prev(cores.upper_bound({time+1, 0}));

        auto [start_time, core]= *core_it;
        int end_time = start_time + omega[v];

        cores.erase(core_it);
        cores.insert({end_time, core});

        schedule.push_back({v, start_time, end_time, core});

        for(int u: ad[v]){
            indeg[u]--;
            max_time[u] = max(max_time[u], end_time);

            if(indeg[u] == 0){
                pq.push({max_time[u], u});
            }
        }
    }

    sort(schedule.begin(), schedule.end());

    out << n << " " << p << " " << gamma << "\n";
    for(auto [task, s, e, c]: schedule){
        out << s << " " << e << " " << c << "\n";
    }
}
