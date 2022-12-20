#include "utils.h"


struct BotCost
{
    int ore{};
    int clay{};
    int obsid{};

    void print() const
    {
        cout << " ore="   << ore; 
        cout << " clay="  << clay; 
        cout << " obsid=" << obsid; 
    }
};


struct BluePrint
{
    BotCost ore_bot{};
    BotCost clay_bot{};
    BotCost obsid_bot{};
    BotCost geode_bot{};

    void print() const
    {
        ore_bot.print(); cout << "\n";
        clay_bot.print(); cout << "\n";
        obsid_bot.print(); cout << "\n";
        geode_bot.print(); cout << "\n";
        cout << "\n";
    }
};


struct State
{
    int minute{};

    int ore_count{};
    int clay_count{};
    int obsid_count{};
    int geode_count{};
    
    int ore_bots{1};
    int clay_bots{};
    int obsid_bots{};
    int geode_bots{};

    void print() const
    {
        cout << " min=" << minute;
        cout << " ore=" << ore_count << "/" << ore_bots;
        cout << " clay=" << clay_count << "/" << clay_bots;
        cout << " obsid=" << obsid_count << "/" << obsid_bots;
        cout << " geode=" << geode_count << "/" << geode_bots;
        cout << " \n";
    }
};


// bool operator<(const State& a, const State& b)
// {
//     if (b.geode_count != a.geode_count) return b.geode_count < a.geode_count;
//     if (b.obsid_count != a.obsid_count) return b.obsid_count < a.obsid_count;
//     if (b.clay_count  != a.clay_count)  return b.clay_count < a.clay_count;
//     return b.ore_count < a.ore_count;
// }


// bool operator<(const State& a, const State& b)
// {

//     if (b.geode_bots != a.geode_bots)   return b.geode_bots < a.geode_bots;
//     if (b.geode_count != a.geode_count) return b.geode_count < a.geode_count;

//     if (b.obsid_bots != a.obsid_bots)   return b.obsid_bots < a.obsid_bots;
//     if (b.obsid_count != a.obsid_count) return b.obsid_count < a.obsid_count;

//     if (b.clay_bots  != a.clay_bots)    return b.clay_bots < a.clay_bots;
//     if (b.clay_count  != a.clay_count)  return b.clay_count < a.clay_count;

//     if (b.ore_bots  != a.ore_bots)      return b.ore_bots < a.ore_bots;
//     return b.ore_count < a.ore_count;
// }


enum Build
{
    None, Geode, Obsid, Clay, Ore
};


bool can_build(const BotCost& cost, const State& s)
{
    if ( (s.ore_count   >= cost.ore)   && 
         (s.clay_count  >= cost.clay)  && 
         (s.obsid_count >= cost.obsid) )
    {
        return true;
    }
    return false;
}


int build(const BotCost& cost, State& s)
{
    if (can_build(cost, s))
    {
        s.ore_count   -= cost.ore;
        s.clay_count  -= cost.clay;
        s.obsid_count -= cost.obsid;
        return 1;
    }
    return 0;
}


State process(const State& state, const BluePrint& bp, Build make)
{
    State s2 = state;

    s2.minute += 1;

    int new_ore_bot   = 0;
    int new_clay_bot  = 0;
    int new_obsid_bot = 0;
    int new_geode_bot = 0;

    switch (make)
    {
        case Ore:             
            new_ore_bot = build(bp.ore_bot, s2);
            break;
        case Clay:    
            new_clay_bot = build(bp.clay_bot, s2); 
            break;       
        case Obsid:     
            new_obsid_bot = build(bp.obsid_bot, s2);
            break;
        case Geode:
            new_geode_bot = build(bp.geode_bot, s2);
            break;
        default:
            break;
    }

    s2.ore_count   += s2.ore_bots;
    s2.clay_count  += s2.clay_bots;
    s2.obsid_count += s2.obsid_bots;
    s2.geode_count += s2.geode_bots;

    s2.ore_bots   += new_ore_bot;
    s2.clay_bots  += new_clay_bot;
    s2.obsid_bots += new_obsid_bot;
    s2.geode_bots += new_geode_bot;

    return s2;
}

constexpr int MINS = 32;


State build_ore_bot(const State& s, const BluePrint& bp)
{
    if (s.ore_bots < max(bp.clay_bot.ore, max(bp.obsid_bot.ore, bp.geode_bot.ore)))
    {
        //cout << "build_ore_bot\n";
        State s2 = s;
        while ((s2.minute < MINS) && (s2.ore_bots == s.ore_bots))
        {
            s2 = process(s2, bp, Ore);
            //s2.print();
        }
        return s2;
    }
    return process(s, bp, None);
}
State build_clay_bot(const State& s, const BluePrint& bp)
{
    if (s.clay_bots < bp.obsid_bot.clay)
    {
        //cout << "build_clay_bot\n";
        State s2 = s;
        while ((s2.minute < MINS) && (s2.clay_bots == s.clay_bots))
        {
            s2 = process(s2, bp, Clay);
            //s2.print();
        }
        return s2;
    }
    return process(s, bp, None);
}
State build_obsid_bot(const State& s, const BluePrint& bp)
{
    if (s.obsid_bots < bp.geode_bot.obsid)
    {
        //cout << "build_obsid_bot\n";
        State s2 = s;
        while ((s2.minute < MINS) && (s2.obsid_bots == s.obsid_bots))
        {
            s2 = process(s2, bp, Obsid);
            //s2.print();
        }
        return s2;
    }
    return process(s, bp, None);
}
State build_geode_bot(const State& s, const BluePrint& bp)
{
    //cout << "build_geode_bot\n";
    State s2 = s;
    while ((s2.minute < MINS) && (s2.geode_bots == s.geode_bots))
    {
        s2 = process(s2, bp, Geode);
        //s2.print();
    }
    return s2;
}


bool operator<(const State& a, const State& b)
{
    if (b.geode_bots != a.geode_bots)   return b.geode_bots < a.geode_bots;
    if (b.obsid_bots != a.obsid_bots)   return b.obsid_bots < a.obsid_bots;
    if (b.clay_bots  != a.clay_bots)    return b.clay_bots < a.clay_bots;
    if (b.ore_bots   != a.ore_bots)     return b.ore_bots < a.ore_bots;
    return b.ore_count < a.ore_count;
}


vector<State> process(const vector<State>& states, const BluePrint& bp, int& geodes)
{
    vector<State> states2;

    for (const auto& state: states)
    {
        auto ore_bot = build_ore_bot(state, bp);
        if (ore_bot.minute == MINS)
            geodes = max(geodes, ore_bot.geode_count);
        else 
            states2.push_back(ore_bot);

        auto clay_bot = build_clay_bot(state, bp);
        if (clay_bot.minute == MINS)
            geodes = max(geodes, clay_bot.geode_count);
        else 
            states2.push_back(clay_bot);

        auto obsid_bot = build_obsid_bot(state, bp);
        if (obsid_bot.minute == MINS)
            geodes = max(geodes, obsid_bot.geode_count);
        else 
            states2.push_back(obsid_bot);

        auto geode_bot = build_geode_bot(state, bp);
        if (geode_bot.minute == MINS)
            geodes = max(geodes, geode_bot.geode_count);
        else 
            states2.push_back(geode_bot);
    }

    constexpr int SIZE = 2'000'000;
    sort(states2.begin(), states2.end());
    if (states2.size() > SIZE)
        states2.erase(states2.begin() + SIZE, states2.end());
    
    return states2;
}


template <typename T>
auto part1(const T& blueprints)
{
    aoc::timer timer;

    int quality = 1; 
    //for (auto b: aoc::range(blueprints.size()))
    //for (auto b: aoc::range(3))
    for (auto b: aoc::range(2))
    {
        const auto& bp = blueprints[b]; 
        //bp.print();

        State state;
        vector<State> states;
        states.push_back(state);

        int geodes = 0;
        while (states.size() > 0)
        {
            states = process(states, bp, geodes);

            cout << "size=" << states.size() << " geodes=" << geodes << "\n";
            // for (auto i: aoc::range(states.size()))
            // {
            //     states[i].print();
            //     if (i > 20) break;
            // }
        }

        //quality += (b + 1) * geodes;
        quality *= geodes;
    }

    return quality;
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    auto ore   = aoc::read_lines<int>(filename,      R"(Each ore robot costs (\d+) ore\.)");
    auto clay  = aoc::read_lines<int>(filename,      R"(Each clay robot costs (\d+) ore\.)");
    auto obsid = aoc::read_lines<int, int>(filename, R"(Each obsidian robot costs (\d+) ore and (\d+) clay\.)");
    auto geode = aoc::read_lines<int, int>(filename, R"(Each geode robot costs (\d+) ore and (\d+) obsidian\.)");

    vector<BluePrint> blueprints;
    for (auto i: aoc::range(ore.size()))
    {
        BluePrint bp;
        bp.ore_bot.ore     = get<0>(ore[i]);       
        bp.clay_bot.ore    = get<0>(clay[i]);
        bp.obsid_bot.ore   = get<0>(obsid[i]);
        bp.obsid_bot.clay  = get<1>(obsid[i]);
        bp.geode_bot.ore   = get<0>(geode[i]);
        bp.geode_bot.obsid = get<1>(geode[i]);
        blueprints.push_back(bp);
    }

    // for (auto bp: blueprints)
    //    bp.print();

    auto p1 = part1(blueprints);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 0);

    auto p2 = part2(blueprints);
    cout << "Part2: " << p2 << '\n';
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
