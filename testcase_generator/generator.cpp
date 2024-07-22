#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <vector>
#include <set> 
#include <cassert>

using namespace std;

vector<pair<int,int>> create_random_dag(int n,int m,mt19937 &rng) {
   
    uniform_int_distribution<> index_gen(0,n-1);

    // Generating a random undirected graph
    set<pair<int,int>> edges;
    for (int i=0;i<m;i++) {
        int x,y;
        do {
            x = index_gen(rng);
            y = index_gen(rng);
        } while(x == y || edges.count(minmax(x,y)));
        edges.insert(minmax(x,y));
    }
    
    // Directing the edges
    vector<int> permutation(n);
    iota(permutation.begin(),permutation.end(),0);
    shuffle(permutation.begin(),permutation.end(),rng);
    vector<pair<int,int>> directed_edges;
    for (auto [x,y]: edges) {
        if (permutation[x] > permutation[y]) { swap(x,y); } 
        directed_edges.emplace_back(x,y);
    }   
    
    return directed_edges;

}

vector<int> get_omega(int n, int p, double min_lambda, double max_lambda, double min_total, double max_total, mt19937 &rng) {
    
    int min_omega = (int)ceil(min_lambda*p);
    int max_omega = (int)floor(max_lambda*p);
    int min_total_omega = (int)ceil(min_total*p);
    int max_total_omega = (int)floor(max_total*p);
    
    uniform_int_distribution<> sum_gen(min_total_omega,max_total_omega);
    uniform_int_distribution<> index_gen(0,n-1);

    
    vector<int> omega(n,min_omega);
    int placed_omega = n * min_omega;
    int target_omega_sum = sum_gen(rng);

    while(placed_omega < target_omega_sum) {
        int index = index_gen(rng);
        if (omega[index] == max_omega) continue;
        omega[index]++;
        placed_omega++;
    }

    return omega;

}

// n,m,p,gamma,lambda_min,lambda_max,sum_lambda_min,sum_lambda_max,seed

int main(int argc,char **argv) {
        
    assert(argc == 2);

    string test_case = argv[1];

    string gen_path = "gen/" + test_case;
    string test_case_path = "tc/" + test_case;

    FILE *in = fopen(gen_path.c_str(),"r");
    FILE *out = fopen(test_case_path.c_str(),"w");

    int n;
    int m;
    int p;
    int gamma;
    double lambda_min;
    double lambda_max;
    double min_total;
    double max_total;
    int seed;

    assert(fscanf(in,"%d %d %d %d %lf %lf %lf %lf %d", &n, &m, &p, &gamma, &lambda_min, &lambda_max, &min_total, &max_total, &seed) == 9);
   
    mt19937 rng(seed);

    vector<pair<int,int>> directed_edges = create_random_dag(n,m,rng);

    vector<int> omega = get_omega(n,p,lambda_min,lambda_max,min_total,max_total,rng);

    fprintf(out,"%d %d %d %d\n",n,m,p,gamma);

    for (int &x: omega) {
        fprintf(out,"%d ",x);
    }
    fprintf(out,"\n");

    for (auto &[x,y]: directed_edges) {
        fprintf(out,"%d %d\n",x,y);
    }

}
