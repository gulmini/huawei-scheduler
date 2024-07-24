#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

mt19937 mrand(chrono::steady_clock::now().time_since_epoch().count());

int rng() {
    return abs((int)mrand());
}

// Current challenge: finish CSES problemset!!

bool isBetter(vector<int> &a,vector<int> &b,int p) {

    int mxa = *max_element(a.begin(),a.end());
    int mna = *min_element(a.begin(),a.end());
    int mxb = *max_element(b.begin(),b.end());
    int mnb = *min_element(b.begin(),b.end());

    if (mxa >= p) { return false; }

    if (mxa == mxb) {
        return (mxa - mna) < (mxb - mnb);
    }

    return mxa < mxb;

}

bool cmin(vector<int> &a,vector<int> &b,int p) {
    if (isBetter(b,a,p)) {
        a = b;
        return true;
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false); 
    cin.tie(nullptr);

    int n,m,p,gamma;
    cin >> n >> m >> p >> gamma;
    vector<int> omega(n);
    for (int i=0;i<n;i++) {
        cin >> omega[i];
    }
    vector<vector<int>> adj(n);
    vector<int> In(n);
    for (int x,y,i=0;i<m;i++) {
        cin >> x >> y;
        adj[x].push_back(y);
        In[y]++;
    }
    const int inf = 1e9;
    vector<vector<int>> servers(1<<n,vector<int>(gamma,inf));
    vector<vector<int>> processes(1<<n,vector<int>(n,inf));
    
    struct item {
        int server;
        int process;
        int st;
        int et;
    };

    vector<item> prev(1<<n,item{-1,-1,-1,-1});
    processes[0].assign(n,0);
    servers[0].assign(gamma,0);
    for (int mask = 0; mask < (1<<n); mask++) {
            
        auto in = In;
        vector<int> lastT(n);

        for (int i=0;i<n;i++) {
            if ((mask >> i) & 1) {
                for (int &y: adj[i]) {
                    --in[y];
                    lastT[y] = max(lastT[y],processes[mask][i]);            
                }
            }
        }

        vector<int> av;    

        for (int i=0;i<n;i++) {
            if ((mask >> i) & 1) { continue; } 
            if (in[i] == 0) { av.push_back(i); }
        }
                
        for (int &new_process: av) {
            
            for (int server = 0; server < gamma; server++) {
                    
                int st = max(servers[mask][server],lastT[new_process]);
                int et = st + omega[new_process];

                auto new_processes = processes[mask];
                auto new_servers = servers[mask];
                
                new_processes[new_process] = et;
                new_servers[server] = et;

                if (cmin(servers[mask|(1<<new_process)],new_servers,p)) {
                    processes[mask|(1<<new_process)] = new_processes;
                    prev[mask|(1<<new_process)] = item{server,new_process,st,et};
                }


            }

        }

    }

    // cout << "went over mask" << endl;

    vector<item> ans(n);
    int mask = (1<<n) - 1;
    while(mask > 0) {
        item lst = prev[mask];
        if (lst.process == -1) {
            cout << "No schedule was found\n";
            return 0;
        }
        ans[lst.process] = lst;
        mask ^= 1<<lst.process;
    }


    cout << n << " " << p << " " << gamma << "\n";
    for (auto &x: ans) {
        cout << x.st << " " << x.et << " " << x.server << "\n";
    }
    

}
