#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <memory>

#include "cxxopts.hpp"

namespace validator
{
    struct input_case
    {
        struct edge
        {
            int src;
            int dst;
        };

        int p, gamma;
        std::vector<int> wcets;
        std::vector<edge> edges;
    };

    class input_case_parser
    {
    private:
        std::istream& stream;

    public:
        input_case_parser(std::istream& stream) : stream(stream) {}

        input_case operator()()
        {
            std::string line;
            std::stringstream ss;

            if (!std::getline(stream, line))
                throw std::invalid_argument("Invalid case header format");
            ss = std::stringstream(line);

            int n, m, p, gamma;
            ss >> n >> m >> p >> gamma;

            if (ss.fail())
                throw std::invalid_argument("Invalid case header format");
            
            input_case out{ p, gamma, std::vector<int>(n), std::vector<input_case::edge>(m) };

            if (!std::getline(stream, line))
                throw std::invalid_argument("Invalid case wcets format");
            ss = std::stringstream(line);

            for (size_t i = 0; i < out.wcets.size(); ++i)
                ss >> out.wcets[i];
            
            if (ss.fail())
                throw std::invalid_argument("Invalid case wcets format");
            
            for (size_t i = 0; i < out.edges.size(); ++i)
            {
                if (!std::getline(stream, line))
                    throw std::invalid_argument("Found less edges than expected");
                ss = std::stringstream(line);

                int src, dst;
                ss >> src >> dst;

                if (ss.fail())
                    throw std::invalid_argument("Invalid case edges format in edge " + std::to_string(i));
                
                out.edges[i] = { src, dst };
            }

            return out;
        }
    };

    struct task_schedule
    {
        int start;
        int end;
        int core;
    };

    struct schedule
    {
        int p, gamma;
        std::vector<task_schedule> tasks;
    };

    class scheduler_parser
    {
    private:
        std::istream& stream;
    
    public:
        scheduler_parser(std::istream& stream) : stream(stream) {}

        schedule operator()()
        {
            std::string line;
            std::stringstream ss;

            if (!std::getline(stream, line))
                throw std::invalid_argument("Invalid schedule header");
            ss = std::stringstream(line);

            int n, p, gamma;
            ss >> n >> p >> gamma;

            if (ss.fail())
                throw std::invalid_argument("Invalid schedule header");
            
            schedule sc{ p, gamma, std::vector<task_schedule>(n) };

            for (size_t i = 0; i < sc.tasks.size(); ++i)
            {
                if (!std::getline(stream, line))
                    throw std::invalid_argument("Found less tasks than expected");
                ss = std::stringstream(line);

                int start, end, core;
                ss >> start >> end >> core;

                if (ss.fail())
                    throw std::invalid_argument("Invalid schedule task " + std::to_string(i));
                
                sc.tasks[i] = { start, end, core };
            }

            return sc;
        }
    };

    class schedule_validator
    {
    public:
        virtual ~schedule_validator() = default;
        virtual void operator()(const input_case& input, const schedule& sched) const = 0;
    };

    class schedule_header_consistency_validator : public schedule_validator
    {
    public:
        virtual void operator()(const input_case& input, const schedule& sched) const
        {
            if (input.p != sched.p || input.gamma != sched.gamma || input.wcets.size() != sched.tasks.size())
                throw std::invalid_argument("Schedule header is not consistent with input header");
        }
    };

    class schedule_core_consistency_validator : public schedule_validator
    {
    public:
        virtual void operator()(const input_case& input, const schedule& sched) const
        {
            for (size_t i = 0; i < sched.tasks.size(); ++i)
                if (sched.tasks[i].core < 0 || sched.tasks[i].core >= sched.gamma)
                    throw std::invalid_argument("Task " + std::to_string(i) + " is assigned to invalid core");
        }
    };

    class schedule_range_consistency_validator : public schedule_validator
    {
    public:
        virtual void operator()(const input_case& input, const schedule& sched) const
        {
            for (size_t i = 0; i < sched.tasks.size(); ++i)
            {
                if (sched.tasks[i].start < 0)
                    throw std::invalid_argument("Task " + std::to_string(i) + " starts at negative time");
                if (sched.tasks[i].end - sched.tasks[i].start != input.wcets[i])
                    throw std::invalid_argument("Task " + std::to_string(i) + "'s scheduled range length is not consistent with task's wcet");
            }
        }
    };

    class schedule_range_overlap_validator : public schedule_validator
    {
    public:
        virtual void operator()(const input_case& input, const schedule& sched) const
        {
            std::vector<std::vector<std::pair<int, int>>> cores(input.gamma);
            for (const task_schedule& i : sched.tasks)
                cores[i.core].push_back({ i.start, i.end });
            for (std::vector<std::pair<int, int>>& i : cores)
            {
                std::sort(i.begin(), i.end());
                for (size_t j = 1; j < i.size(); ++j)
                    if (i[j - 1].second > i[j].first)
                        throw std::invalid_argument("Overlapping tasks");
            }
        }
    };

    class schedule_precedence_validator : public schedule_validator
    {
    public:
        virtual void operator()(const input_case& input, const schedule& sched) const
        {
            for (const input_case::edge& i : input.edges)
                if (sched.tasks[i.src].end > sched.tasks[i.dst].start)
                    throw std::invalid_argument("Task " + std::to_string(i.dst) + " depends on task " + std::to_string(i.src) + " but starts before it ends");
        }
    };
}

int main(int argc, char* argv[])
{
    cxxopts::Options options("Schedule Validator", "Validates a schedule over a given input");

    options.add_options()
        ("scheduler", "Input scheduler name", cxxopts::value<std::string>())
        ("case", "Input case number", cxxopts::value<std::string>())
        ("h,help", "Print help message");
    options.parse_positional({ "scheduler" , "case" });
    cxxopts::ParseResult cmdline = options.parse(argc, argv);

    if (cmdline.count("help") || !cmdline.count("case") || !cmdline.count("scheduler"))
    {
        std::cout << options.help() << std::endl;
        return 1;
    }

    validator::input_case input;

    try
    {
        std::string input_case_path = std::getenv("SCHED_TC_FOLDER") + ("/" + cmdline["case"].as<std::string>()) + ".tc";
        std::ifstream s(input_case_path);
        validator::input_case_parser case_parser(s);
        input = case_parser();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Could not parse case file: " << e.what() << std::endl;
        return 1;
    }

    validator::schedule sched;

    try
    {
        std::string schedule_path = std::getenv("SCHED_OUTPUT_FOLDER") + ("/" + cmdline["scheduler"].as<std::string>()) + ("/" + cmdline["case"].as<std::string>()) + ".sched";
        std::ifstream s(schedule_path);
        validator::scheduler_parser schedule_parser(s);
        sched = schedule_parser();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Could not parse schedule file: " << e.what() << std::endl;
        return 1;
    }

    std::map<std::string, std::unique_ptr<validator::schedule_validator>> validators;

    validators["schedule_header_consistency"] = std::unique_ptr<validator::schedule_header_consistency_validator>(new validator::schedule_header_consistency_validator());
    validators["schedule_core_consistency"] = std::unique_ptr<validator::schedule_core_consistency_validator>(new validator::schedule_core_consistency_validator());
    validators["schedule_range_consistency"] = std::unique_ptr<validator::schedule_range_consistency_validator>(new validator::schedule_range_consistency_validator());
    validators["schedule_ranger_overlap"] = std::unique_ptr<validator::schedule_range_overlap_validator>(new validator::schedule_range_overlap_validator());
    validators["schedule_precedence"] = std::unique_ptr<validator::schedule_precedence_validator>(new validator::schedule_precedence_validator());


    std::vector<std::string> torun {
        "schedule_header_consistency",
        "schedule_core_consistency",
        "schedule_range_consistency",
        "schedule_ranger_overlap",
        "schedule_precedence"
    };

    for (const std::string& s : torun)
    {
        try
        {
            validators[s]->operator()(input, sched);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Validator " << s << " failed: " << e.what() << std::endl;
            return 1;
        }
    }
}