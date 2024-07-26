#include <vector>
#include <algorithm>
#include <cassert>
#include <array>
using namespace std;

const char *scheduler_name = "papera_matta_satsolver";

bool isBetter(vector<int> &a,vector<int> &b) {

    int mxa = *max_element(a.begin(),a.end());
    int mna = *min_element(a.begin(),a.end());
    int mxb = *max_element(b.begin(),b.end());
    int mnb = *min_element(b.begin(),b.end());

    if (mxa == mxb) {
        return (mxa - mna) < (mxb - mnb);
    }

    return mxa < mxb;

}

bool cmin(vector<int> &a,vector<int> &b) {
    if (isBetter(b,a)) {
        a = b;
        return true;
    }
    return false;
}

vector<array<int,3>> schedule(int n,int m,int p,int gamma,vector<int> omega,vector<int> a,vector<int> b) {
    vector<vector<int>> adj(n);
    vector<int> In(n);
    for (int i=0;i<m;i++) {
        int x = a[i];
        int y = b[i];
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

                if (cmin(servers[mask|(1<<new_process)],new_servers)) {
                    processes[mask|(1<<new_process)] = new_processes;
                    prev[mask|(1<<new_process)] = item{server,new_process,st,et};
                }


            }

        }

    }

    vector<array<int,3>> ans(n);
    int mask = (1<<n) - 1;
    while(mask > 0) {
        item lst = prev[mask];
        assert(lst.process!=-1);
        ans[lst.process] = {lst.st,lst.et,lst.server};
        mask ^= 1<<lst.process;
    }

    return ans;
}
