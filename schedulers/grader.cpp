#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <array>

using namespace std;

extern const char* scheduler_name;

vector<array<int, 3>> schedule(int N, int M, int P, int Gamma, vector<int> Omega, vector<int> A, vector<int> B);

int main(int argc, char **argv){
    assert(argc >= 2);
    string testcase = argv[1];

    char *tc_folder, *output_folder;
    assert(tc_folder = std::getenv("SCHED_TC_FOLDER"));
    assert(output_folder = std::getenv("SCHED_OUTPUT_FOLDER"));

    string testcase_path = tc_folder + ("/" + testcase) + ".tc";
    string output_path = output_folder + ("/" + (scheduler_name + ("/" + testcase)) + ".sched");

    ifstream in(testcase_path);
    ofstream out(output_path);

    assert(in);

    int N, M, P, Gamma;
    in >> N >> M >> P >> Gamma;

    vector<int> Omega(N), A(M), B(M);

    for(int &x: Omega) in >> x;
    for(int i=0; i<M; i++){
        in >> A[i] >> B[i];
    }

    vector<array<int, 3>> sched = schedule(N, M, P, Gamma, Omega, A, B);
    assert(sched.size() == N);

    out << N << " " << P << " " << Gamma << "\n";
    for(auto [s, e, c]: sched){
        out << s << " " << e << " " << c << "\n";
    }
}
