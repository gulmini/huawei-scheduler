#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <utility>
#include <limits>

struct test_case
{
    int period;
    int cores;

    std::vector<int> wcet;
    std::vector<std::vector<int>> precedences;
};

test_case parse_case_file(std::string path)
{
    std::FILE* file = std::fopen(path.c_str(), "r");
    assert(file);
    
    int n, m, p, gamma;

    assert(std::fscanf(file, "%d %d %d %d", &n, &m, &p, &gamma) == 4);
    
    test_case tc { p, gamma, std::vector<int>(n), std::vector<std::vector<int>>(n) };

    for (int i = 0; i < n; ++i)
        assert(std::fscanf(file, "%d", &tc.wcet[i]) == 1);
    
    for (int i = 0; i < m; ++i)
    {
        int src, dst;
        assert(std::fscanf(file, "%d %d", &src, &dst) == 2);
        assert(src >= 0 && src < n && dst >= 0 && dst < n);
        tc.precedences[src].push_back(dst);
    }

    return tc;
}

void validate_schedule_file(test_case tc, std::string path)
{
    std::FILE* file = std::fopen(path.c_str(), "r");
    assert(file);
    
    int n;

    assert(std::fscanf(file, "%d", &n) == 1);
    
    std::vector<std::vector<std::pair<int, int>>> cores(tc.cores);
    std::vector<std::pair<int, int>> segments(n);

    for (int i = 0; i < n; ++i)
    {
        int core, start, end;
        assert(std::fscanf(file, "%d %d %d", &start, &end, &core) == 3);
        assert(core < tc.cores);
        assert (end - start == tc.wcet[i]);
        cores[core].push_back({ start, end });
        segments[i] = { start, end };
    }

    int end = std::numeric_limits<int>::min();

    // Check for overlapping tasks on the same core.
    for (std::vector<std::pair<int, int>>& core : cores)
    {
        std::sort(core.begin(), core.end());
        for (int i = 1; i < core.size(); ++i)
            assert(core[i].first > core[i - 1].second);
        if (!core.empty())
            end = std::max(end, core.back().second);
    }
    
    // Check for broken dependencies.
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < tc.precedences[i].size(); ++j)
            assert(segments[i].second < segments[tc.precedences[i][j]].first);
    assert(end < tc.period);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Bad arguments number. Got " << argc - 1 << ", expected " << 2 << "." << std::endl
            << "Correct parameters: <case_path> <schedule_path>" << std::endl;
        return 1;
    }

    test_case case_parse = parse_case_file(argv[1]);

    validate_schedule_file(case_parse, argv[2]);
}