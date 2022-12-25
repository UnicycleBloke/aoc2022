#include "utils.h"

size_t decode_snafu(const string& s)
{
    size_t d = 0;
    for (auto c: s)
    {
        d *= 5;
        switch (c)
        {
            case '2': d += 2; break;
            case '1': d += 1; break;
            case '0': d += 0; break;
            case '-': d -= 1; break;
            case '=': d -= 2; break;
        }
    }
    return d;
}


string encode_snafu(size_t s)
{
    // Convert to base-5. p[0] is the 1s, p[1] is the 5s, ....
    vector<int> p;
    do
    {
        p.push_back(s % 5);
        s /= 5;
    }
    while (s > 0);

    // Allow for more digits just in case
    p.push_back(0);
    p.push_back(0);
    p.push_back(0);

    // Convert to snafu digits
    for (auto i: aoc::range(p.size()-1))
    {
        if (p[i] > 2)
        {
            p[i]   -= 5;
            p[i+1] += 1;
        }
    }

    // Render the snafu as a string
    // Skip leading zeroes
    int i = (int)p.size()-1;
    while (p[i] == 0) --i;

    string r;
    do 
    {
        switch (p[i])
        {
            case -2: r.push_back('='); break;
            case -1: r.push_back('-'); break;
            case  0: r.push_back('0'); break;
            case  1: r.push_back('1'); break;
            case  2: r.push_back('2'); break;
        } 
        --i;
    }
    while (i > -1);
    return r;
}


template <typename T>
auto part1(const T& input)
{
    aoc::timer timer;

    size_t sum = 0;    
    for (const auto& s: input)
    {
        auto d = decode_snafu(s);
        // auto e = encode_snafu(d);
        // cout << s << " " << d << " " << e << "\n";
        sum += d;
    }

    auto res = encode_snafu(sum);
    // cout << sum << "\n" << res << "\n";
    return res;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, aoc::Blanks::Suppress, aoc::Trim::Yes);

    auto p1 = part1(lines);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, "121=2=1==0=10=2-20=2"); // 27210103880867 121=2=1==0=10=2-20=2
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
