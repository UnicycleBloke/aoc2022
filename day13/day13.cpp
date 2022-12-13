#include "utils.h"


vector<string> split_msg(const string& s)
{
    vector<string> res;
    string temp;
    int brackets = 0;
    for (auto c: s)
    {
        if (c == ',')
        {
            if (brackets == 0)
            {
                res.push_back(temp);
                temp.clear();
            }
            else
            {
                temp.push_back(c);
            }
        }
        else
        {
            temp.push_back(c);
            if (c == '[') ++brackets;
            if (c == ']') --brackets;
        }
    }
    res.push_back(temp);

    return res;
}


int list_compare(const vector<string>& left, const vector<string>& right)
{
    size_t i = 0;

    while (i < min(left.size(), right.size()))
    {
        const auto& l = left[i];
        const auto& r = right[i];
        ++i;

        if ((l[0] == '[') && (r[0] == '['))
        {
            auto ls  = split_msg(l.substr(1, l.size()-2));
            auto rs  = split_msg(r.substr(1, r.size()-2));
            auto res = list_compare(ls, rs);
            if (res != 0) 
                return res;
        }
        else if ((l[0] == '['))
        {
            auto ls  = split_msg(l.substr(1, l.size()-2));
            vector<string> rs{r};
            auto res = list_compare(ls, rs);
            if (res != 0) 
                return res;
        }
        else if ((r[0] == '['))
        {
            auto rs  = split_msg(r.substr(1, r.size()-2));
            vector<string> ls{l};
            auto res = list_compare(ls, rs);
            if (res != 0) 
                return res;
        }
        else 
        { 
            if ((l.size() == 0) && (r.size() == 0)) return 0;
            if (l.size() == 0) return 1;
            if (r.size() == 0) return -1;

            int lv = stoi(l);
            int rv = stoi(r);
            if (lv != rv)
                return aoc::sgn(rv - lv);
        }
    }

    if (left.size() < right.size()) return 1;
    if (left.size() > right.size()) return -1;
    return 0;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    size_t sum = 0;
    size_t i = 0;
    while (i < input.size())
    {
        auto left  = input[i++];
        auto right = input[i++];
        i++;

        auto res = list_compare(vector<string>{left}, vector<string>{right});
        if (res == 1)
           sum += (i/3);
    }

    return sum;
}


template <typename T>
auto part2(const T& input)
{
    aoc::timer timer;

    vector<string> all;
    size_t i = 0;
    while (i < input.size())
    {
        all.push_back(input[i++]);
        all.push_back(input[i++]);
        i++;
    }
    all.push_back("[[2]]");
    all.push_back("[[6]]");

    sort(all.begin(), all.end(), [](const string& left, const string& right)
    {
        auto res = list_compare(vector<string>{left}, vector<string>{right});
        return res == 1;
    });

    auto a = find(all.begin(), all.end(), "[[2]]") - all.begin() + 1;
    auto b = find(all.begin(), all.end(), "[[6]]") - all.begin() + 1;

    return a * b;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Allow, aoc::Trim::No);

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 5013U);

    auto p2 = part2(lines);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 25038);
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
