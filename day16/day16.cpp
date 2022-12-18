#include "utils.h"


struct State
{
    string      valve{"AA"};
    set<string> open{};
    int         total{};
    int         rate{};
    int         delay{};
};


struct State2
{
    // Shared
    set<string> open{};
    int         total{};
    int         rate{};
    // Me 
    string      i_valve{"AA"};
    int         i_delay{};
    // The elephant
    string      e_valve{"AA"};
    int         e_delay{};
};


bool operator<(const State& lhs, const State& rhs)
{
    if (lhs.total > rhs.total) return true;
    return false;
}

bool operator<(const State2& lhs, const State2& rhs)
{
    if (lhs.total > rhs.total) return true;
    return false;
}


int cost(map<pair<string, string>, int>& edges, const string& from, const string& to)
{
    auto e = pair{from, to};                 
    if (e.second < e.first)
        e = pair{e.second, e.first};  
    return edges[e];
}


template <typename T>
auto part1(T& input, string first, map<pair<string, string>, int>& edges)
{
    aoc::timer timer;
 
    vector<State> states;
    State start;
    start.valve = first;
    states.push_back(start);

    for (int minute: aoc::range(1, 31))
    {
        vector<State> states2;

        for (const auto& state: states)
        {
            const auto& [rate, valves] = input[state.valve];
            bool open = state.open.find(state.valve) != state.open.end();

            if (state.delay > 0)
            {
                State state2 = state;
                state2.total += state2.rate;
                --state2.delay;
                states2.push_back(state2);
            }
            else
            {
                if (!open && (rate > 0))
                {
                    State state2 = state;
                    state2.total += state2.rate;
                    state2.open.insert(state.valve);
                    state2.rate  += rate;
                    states2.push_back(state2);
                }

                for (const auto& valve2: valves)
                {
                    State state2  = state;
                    state2.total += state2.rate;
                    state2.valve  = valve2;
                    state2.delay  = cost(edges, state.valve, valve2) - 1;
                    states2.push_back(state2);
                }
            }
        }

        constexpr int SIZE = 5'000;

        states = std::move(states2);
        sort(states.begin(), states.end());
        if (states.size() > SIZE)
            states.erase(states.begin() + SIZE, states.end());
    }

    return states[0].total;
}


template <typename T>
auto part2(T& input, string first, map<pair<string, string>, int>& edges)
{
    aoc::timer timer;
 
    vector<State2> states;
    State2 start;
    start.i_valve = first;
    start.e_valve = first;
    states.push_back(start);

    for (int minute: aoc::range(1, 27))
    {
        vector<State2> states2;

        auto elephant = [&](const State2& state)
        {
            const auto& [rate, valves] = input[state.e_valve];
            bool open = state.open.find(state.e_valve) != state.open.end();

            if (state.e_delay > 0)
            {
                State2 state2 = state;
                //state2.total += state2.rate;
                --state2.e_delay;
                states2.push_back(state2);
            }
            else
            {
                if (!open && (rate > 0))
                {
                    State2 state2 = state;
                    //state2.total += state2.rate;
                    state2.open.insert(state.e_valve);
                    state2.rate  += rate;
                    states2.push_back(state2);
                }

                for (const auto& valve2: valves)
                {
                    State2 state2  = state;
                    //state2.total += state2.rate;
                    state2.e_valve  = valve2;
                    state2.e_delay  = cost(edges, state.e_valve, valve2) - 1;
                    states2.push_back(state2);
                }
            }
        };

        auto user = [&](const State2& state)
        {
            const auto& [rate, valves] = input[state.i_valve];
            bool open = state.open.find(state.i_valve) != state.open.end();

            if (state.i_delay > 0)
            {
                State2 state2 = state;
                state2.total += state2.rate;
                --state2.i_delay;
                elephant(state2);
            }
            else
            {
                if (!open && (rate > 0))
                {
                    State2 state2 = state;
                    state2.total += state2.rate;
                    state2.open.insert(state.i_valve);
                    state2.rate  += rate;
                    elephant(state2);
                }

                for (const auto& valve2: valves)
                {
                    State2 state2  = state;
                    state2.total += state2.rate;
                    state2.i_valve  = valve2;
                    state2.i_delay  = cost(edges, state.i_valve, valve2) - 1;
                    elephant(state2);
                }
            }
        };

        for (const auto& state: states)        
        {
            // Compose the action of the the user (me) and the elephant the user 
            // calling the elephant with an updated state for each new path, and the 
            // elephant then appending its modified versions of that to the state vector.
            // This kind of squares the number of expected states for each minute.
            user(state);
        }

        constexpr int SIZE = 5'000;

        states = std::move(states2);
        sort(states.begin(), states.end());
        if (states.size() > SIZE)
            states.erase(states.begin() + SIZE, states.end());
    }

    return states[0].total;
}


// Remove the nodes with zero rate to reduce the massive explosion of paths we will get to 
// a manageable scale. Replace the intermediate nodes with a cost of moving along each edge.
// It took a ridiculous amount of time to get the logic for this right - my brain was in
// hibernation during a cold snap, or something.
auto reduce(const char* filename)
{
    map<pair<string, string>, int> edges;
    map<string, int> rates;

    auto lines = aoc::read_lines(filename);
    for (auto line: lines)
    {
        line = aoc::replace(line, ", ", ",");
        auto [v1, rate, vs] = aoc::parse_line<string, int, string>(R"(Valve (\w\w) has flow rate=(\d+); tunnels? leads? to valves? (.*))", line);
        auto valves = aoc::split(vs, ",", false);

        for (const auto& v2: valves)
        {
            if (v1 < v2)
                edges[{v1, v2}] = 1;
            else
                edges[{v2, v1}] = 1;
        }
        rates[v1] = rate;
    }

    auto combine = [&] 
    {
        for (const auto& [e1, c1]: edges)
        {
            if ((rates[e1.first] == 0) && (rates[e1.second] == 0)) continue;
            if ((rates[e1.first] != 0) && (rates[e1.second] != 0)) continue;
            // One and only one end is a zero

            for (const auto& [e2, c2]: edges)
            {
                if (e1 == e2) continue;
                if ((rates[e2.first] != 0) && (rates[e2.second] != 0)) continue;
                // Either end could be a zero

                pair<string, string> e;
                bool match = false;

                if ((rates[e1.first] == 0) && (rates[e2.first] == 0) && (e1.first == e2.first))
                {
                    e = pair{e1.second, e2.second}; 
                    match = true;
                }

                else if ((rates[e1.second] == 0) && (rates[e2.first] == 0) && (e1.second == e2.first))
                {
                    e = pair{e1.first, e2.second};  
                    match = true;
                }

                else if ((rates[e1.first] == 0) && (rates[e2.second] == 0) && (e1.first == e2.second))
                {
                    e = pair{e1.second, e2.first}; 
                    match = true;
                }

                else if ((rates[e1.second] == 0) && (rates[e2.second] == 0) && (e1.second == e2.second))
                {
                    e = pair{e1.first, e2.first};                 
                    match = true;
                }

                if (!match) continue;  

                edges.erase(e1);
                edges.erase(e2);

                if (e.second < e.first)
                    e = pair{e.second, e.first};  
                edges[e] = c1 + c2;
                return true;
            }
        }
        return false;
    };

    // To prevent removal.
    rates["AA"] = 1;
    int removed = 0;
    while (combine())
        ++removed;
    rates["AA"] = 0;

    return pair{edges, rates};
} 


struct Cave
{
    int rate{};
    vector<string> valves{};
};


void run(const char* filename)
{
    auto [edges, rates] = reduce(filename);

    map<string, Cave> input;

    for (const auto& [edge, cost]: edges)
    {
        const auto& [v1, v2] = edge;
        auto rate = rates[v1];

        auto& data1 = input[v1];
        data1.rate  = rates[v1];
        data1.valves.push_back(v2);

        auto& data2 = input[v2];
        data2.rate  = rates[v2];
        data2.valves.push_back(v1);
    }

    // Graphviz DOT graph
    // for (const auto& [valve, value]: input)
    // {
    //     const auto& [rate, valves] = value;
    //     for (auto v: valves)
    //         cout << valve << " -> " << v << " [label=\"" << cost(edges, valve, v) << "\"];\n";
    //     cout << valve << " [label=\"" << valve << " " << rate << "\"";
    //     if (rate > 0)
    //     {
    //         cout << ", fontcolor=red, color=red";
    //     }
    //     cout << "];";
    //     cout << "\n";
    // }    

    auto p1 = part1(input, "AA", edges);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 2077);

    auto p2 = part2(input, "AA", edges);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 2741);
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
