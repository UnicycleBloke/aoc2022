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
    int ore_count{};
    int clay_count{};
    int obsid_count{};
    int geode_count{};
    
    int ore_bots{1};
    int clay_bots{};
    int obsid_bots{};
    int geode_bots{};
};


// bool operator<(const State& a, const State& b)
// {
//     if (b.geode_count != a.geode_count) return b.geode_count < a.geode_count;
//     if (b.obsid_count != a.obsid_count) return b.obsid_count < a.obsid_count;
//     if (b.clay_count  != a.clay_count)  return b.clay_count < a.clay_count;
//     return b.ore_count < a.ore_count;
// }


bool operator<(const State& a, const State& b)
{

    if (b.geode_bots != a.geode_bots)   return b.geode_bots < a.geode_bots;
    if (b.geode_count != a.geode_count) return b.geode_count < a.geode_count;

    if (b.obsid_bots != a.obsid_bots)   return b.obsid_bots < a.obsid_bots;
    if (b.obsid_count != a.obsid_count) return b.obsid_count < a.obsid_count;

    if (b.clay_bots  != a.clay_bots)    return b.clay_bots < a.clay_bots;
    if (b.clay_count  != a.clay_count)  return b.clay_count < a.clay_count;

    if (b.ore_bots  != a.ore_bots)      return b.ore_bots < a.ore_bots;
    return b.ore_count < a.ore_count;
}


void build(const BotCost& cost, State& s)
{
    s.ore_count   -= cost.ore;
    s.clay_count  -= cost.clay;
    s.obsid_count -= cost.obsid;
}


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


State process(const State& state, const BluePrint& bp, Build make)
{
    State s2 = state;

    int new_ore_bot   = 0;
    int new_clay_bot  = 0;
    int new_obsid_bot = 0;
    int new_geode_bot = 0;

    switch (make)
    {
        case Ore:
            build(bp.ore_bot, s2); 
            new_ore_bot = 1;
            break;
        case Clay:    
            build(bp.clay_bot, s2); 
            new_clay_bot = 1;
            break;       
        case Obsid:     
            build(bp.obsid_bot, s2);
            new_obsid_bot = 1;
            break;
        case Geode:
            build(bp.geode_bot, s2);
            new_geode_bot = 1;
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


vector<State> process(const vector<State>& states, const BluePrint& bp)
{
    vector<State> states2;

    for (const auto& state: states)
    {
        if (can_build(bp.geode_bot, state))
            states2.push_back(process(state, bp, Geode));
        if (can_build(bp.obsid_bot, state))
            states2.push_back(process(state, bp, Obsid));
        if (can_build(bp.clay_bot, state))
            states2.push_back(process(state, bp, Clay));
        if (can_build(bp.ore_bot, state))
            states2.push_back(process(state, bp, Ore));
        states2.push_back(process(state, bp, None));
    }

    constexpr int SIZE = 200'000;

    sort(states2.begin(), states2.end());
        if (states2.size() > SIZE)
            states2.erase(states2.begin() + SIZE, states2.end());
    
    return states2;
}


template <typename T>
auto part1(const T& blueprints)
{
    aoc::timer timer;

    int quality = 0; 
    for (auto b: aoc::range(blueprints.size()))
    {
        const auto& bp = blueprints[b]; 
        bp.print();

        State state;
        vector<State> states;
        states.push_back(state);

        for (auto i: aoc::range(1, 25))
        {
            states = process(states, bp);

            cout << "Minute " << i << " size=" << states.size() << "\n";
            for (auto i: aoc::range(states.size()))
            {
                cout << " ore="   << states[0].ore_bots   << "/" << states[i].ore_count; 
                cout << " clay="  << states[0].clay_bots  << "/" << states[i].clay_count; 
                cout << " obsid=" << states[0].obsid_bots << "/" << states[i].obsid_count; 
                cout << " geode=" << states[0].geode_bots << "/" << states[i].geode_count;
                cout << "\n"; 

                if (i > 5) break;
            }
        }

        quality += (b + 1) * states[0].geode_count;
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
