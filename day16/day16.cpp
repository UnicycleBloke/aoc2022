#include "utils.h"


struct State
{
    string      valve{"AA"};
    set<string> open{};
    int         total{};
    int         rate{};
    int         delta{};
};

bool operator<(const State& lhs, const State& rhs)
{
    if (lhs.total > rhs.total) return true;
    //if (lhs.possible > rhs.possible) return true;
    // if (lhs.rate > rhs.rate) return true;
    // if ((lhs.rate == rhs.rate) && (lhs.total > rhs.total)) return true;
    return false;;
}


template <typename T>
auto part1(T& input, string first)
{
    aoc::timer timer;
 
    vector<State> states;
    State start;
    start.valve = first;
    states.push_back(start);
    cout << first << "\n";

    for (int minute: aoc::range(1, 31))
    {
        vector<State> states2;

        cout << "\nMinute " << minute <<" size=" << states.size() << "\n\n";

        for (const auto& state: states)
        {
            const auto& [valve, rate, valves] = input[state.valve];
            bool open = state.open.find(valve) != state.open.end();

            if (!open && (rate > 0))
            {
                State state2 = state;
                state2.total += state2.rate;

                state2.open.insert(valve);
                state2.delta  = rate;
                state2.rate  += rate;

                states2.push_back(state2);

                // if (valve == string{"JJ"})
                // {
                //     cout << state.valve << " opened " << state2.rate << " " << state2.total << " (";
                //     for (const auto& o: state2.open)
                //         cout << o << " ";
                //     cout << ")\n";
                // }
            }

            for (const auto& valve2: valves)
            {
                State state2  = state;
                state2.delta  = 0;
                state2.total += state2.rate;
                state2.valve  = valve2;

                states2.push_back(state2);

                // cout << state.valve << " => " << state2.valve << " " << state2.rate << " " << state2.total << " (";
                // for (const auto& o: state2.open)
                //     cout << o << " ";
                // cout << ")\n";
            }
        }

        constexpr int SIZE = 100000;

        sort(states2.begin(), states2.end());
        if (states2.size() > SIZE)
            states2.erase(states2.begin() + SIZE, states2.end());
        states = std::move(states2);

        cout << "Minute " << minute << " " << states[0].rate << " " << states[0].total << " ";
        cout << "( ";
        for (const auto& o: states[0].open)
            cout << o << " ";
        cout << ")\n";
    }

    return 0;
}


template <typename T>
auto part2(T& input)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    using Input = tuple<int, vector<string>>;
    map<string, Input> input;

    auto lines = aoc::read_lines(filename);
    for (auto line: lines)
    {
        line = aoc::replace(line, ", ", ",");
        auto [valve, rate, vs] = aoc::parse_line<string, int, string>(R"(Valve (\w\w) has flow rate=(\d+); tunnels? leads? to valves? (.*))", line);
        auto valves = aoc::split(vs, ",", false);
        input[valve] = make_tuple(rate, valves);
    }

    // for (const auto& [valve, value]: input)
    // {
    //     const auto& [rate, valves] = value;
    //     cout << valve << " " << rate << " "; 
    //     for (auto v: valves)
    //         cout << v << " ";
    //     cout << "\n";
    // }

    // for (const auto& [valve, value]: input)
    // {
    //     const auto& [rate, valves] = value;
    //     for (auto v: valves)
    //         cout << valve << " -> " << v << ";\n";
    //     cout << valve << " [label=\"" << valve << " " << rate << "\"";
    //     if (rate > 0)
    //     {
    //         cout << ", fontcolor=red, color=red";
    //     }
    //     cout << "];";
    //     cout << "\n";
    // }
    
    map<pair<string, string>, int> links;
    for (const auto& [v1, value1]: input)
    {
        for (const auto& [v2, value2]: input)
        {
            links[{v1, v2}] = 1'000'000; 
        }
 
        const auto& [rate1, valves1] = value1;
        for (auto v2: valves1)
        {
            links[{v1, v2}] = 1; 
        }
    }

    while (true)
    {
        for (const auto& [from, value]: input)
        {
            const auto& [rate, valves] = value;
            for (auto to: valves)
            {
                int d = links;
            }
                links[{from, to}] = 1; 
        }
    }



    // auto p1 = part1(input, start);
    // cout << "Part1: " << p1 << '\n';
    // //aoc::check_result(p1, 0);

    // auto p2 = part2(input);
    // cout << "Part2: " << p2 << '\n';
    //aoc::check_result(p2, 0);
}


int main(int argc, char** argv)
{
    aoc::timer timer;
    try
    {
        if (argc < 2)
        {
            cout << "Provide input file name\n";
            return -1;
        }

        fs::path path{argv[1]};
        if (!fs::is_regular_file(path))
        {
            cout << "Path '" << argv[1] << "' does not exist or is not a file\n";
            return -1;
        }

        run(argv[1]);
    }
    catch (std::exception& e)
    {
        cout << e.what();
    }
}
