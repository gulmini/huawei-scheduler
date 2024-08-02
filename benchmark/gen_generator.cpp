#include <random>
#include <chrono>
#include <cassert>
#include <iostream>
using namespace std;
mt19937 mrand(chrono::steady_clock::now().time_since_epoch().count());

constexpr double eps = 1e-9;

bool leq(double a,double b) {
    return a-b <= eps;
}

int main() {
    
    uniform_int_distribution<> gen_low(1,3);
    uniform_int_distribution<> gen_high(80,99);
    
    uniform_int_distribution<> gen_tot_low(2,3);
    uniform_int_distribution<> gen_tot_high(3,4);

    double lambda_min = gen_low(mrand);
    lambda_min /= 100;
    double lambda_max = gen_high(mrand);
    lambda_max /= 100;

    double min_total = gen_tot_low(mrand);
    double max_total = gen_tot_high(mrand);

    assert(leq(0,lambda_min) && leq(lambda_min,lambda_max) && leq(lambda_max,1));
    assert(leq(min_total,max_total));
   

    // assert(ln % sn == 0 && rn % sn == 0 && lm % sm == 0 && rm % sm == 0);
    
    int p = 100, gamma = 4, seed = 42069;

    vector<int> n_steps = { 30, 40, 50, 60, 70 };
    vector<int> m_steps = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300 };

    for (auto n : n_steps) {
        for (auto m : m_steps) {
            bool good = true;
            // good &= (m <= (long long)n*(n-1)/2);
            // good &= (leq(0,lambda_min) && leq(lambda_min,lambda_max) && leq(lambda_max,1));
            // good &= (leq(n * lambda_min,min_total) && leq(min_total,max_total) && leq(max_total,n * lambda_max));
            if (good) {
                cout << n << " " << m << " " << p << " " << gamma 
                    << " " << lambda_min << " " << lambda_max << " " << min_total << " " << max_total << " " << seed << "\n"; 
            }
        }  

    }   

}
