#include "utils.h"


struct File
{
    size_t size;
    string name;
};


struct Directory
{
    string name{};
    Directory* parent{};
    map<string, Directory*> dirs{};
    vector<File> files{};

    size_t total_size()
    {
        size_t result = 0;
        for (const auto& [k, v]: dirs)
            result += v->total_size();
        for (const auto f: files)
            result += f.size;
        return result;
    }

    Directory* cd(string name)
    {
        if (dirs.find(name) == dirs.end())
        {
            Directory* d = new Directory;
            d->name    = name;
            d->parent  = this;
            dirs[name] = d;
        }
        
        return dirs[name];
    }

    size_t small_dirs()
    {
        size_t size   = total_size();
        size_t result = (size <= 100'000U) ? size : 0U;
        for (const auto& [name, dir]: dirs)
            result += dir->small_dirs();
        return result;
    }

    void free_dir(size_t free, vector<size_t>& sizes)
    {
        size_t size = total_size();
        if (size >= free) 
           sizes.push_back(size);
        for (const auto& [name, dir]: dirs)
            dir->free_dir(free, sizes);
    }
};



auto part1(Directory& fs)
{
    aoc::timer timer;
    return fs.small_dirs();
}


auto part2(Directory& fs)
{
    aoc::timer timer;

    constexpr size_t DISK_SPACE   = 70'000'000;
    constexpr size_t UPDATE_SPACE = 30'000'000;

    size_t used_space   = fs.total_size();
    size_t unused_space = DISK_SPACE - used_space;
    size_t free_space   = UPDATE_SPACE - unused_space;

    vector<size_t> sizes;
    fs.free_dir(free_space, sizes);
    sort(sizes.begin(), sizes.end());

    return *sizes.begin();
}


void run(const char* filename)
{
    //auto lines = aoc::read_lines<int,int,int,int>(filename, R"((\d+)-(\d+),(\d+)-(\d+))");
    auto lines = aoc::read_lines(filename, aoc::Blanks::Allow, aoc::Trim::No);

    Directory  fs;
    Directory* wd = &fs;

    for (const auto& line: lines)
    {
        auto s = aoc::split(line, " ");
        if (line[0] == '$')
        {
            if (s[1].compare("cd") == 0)
            {
                if (s[2].compare("/") == 0)
                {
                    wd = &fs;
                }
                else if (s[2].compare("..") == 0)
                {
                    wd = wd->parent;
                }
                else
                {
                    wd = wd->cd(s[2]);
                }
            } 
        }
        else
        {
            if (s[0].compare("dir") == 0)
            {
                wd->cd(s[1]);
            }
            else
            {
                File f;
                f.size = stoul(s[0]);
                f.name = s[1];
                wd->files.push_back(f);
            }
        } 
    }

    auto p1 = part1(fs);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 1723892U);

    auto p2 = part2(fs);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 8474158U);
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
