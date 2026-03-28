#include <iostream>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <fstream>
#include <string_view>
#include <random>

struct range
{
    std::uint32_t start_point{};
    std::uint32_t end_point{};
    std::uint32_t value_count{};

};

class range_set
{

private:
    std::uint32_t m_max_value_count{};
    std::uint32_t m_max_point_distance{};

    std::vector<range> m_ranges{};

    //compare ranges
    bool compare_ranges(const range& a, const range& b)
    {
        if (a.start_point != b.start_point)
        {
            return a.start_point < b.start_point;
        }
        return a.end_point < b.end_point;
    }
    bool can_merge(const range& a, const range& b) const
    {
        std::uint64_t merged_start{std::min(a.start_point, b.start_point)};
        std::uint64_t merged_end{std::max(a.end_point, b.end_point)};
        std::uint64_t merged_value_count{static_cast<std::uint64_t>(a.value_count) + static_cast<std::uint64_t>(b.value_count)};
        return merged_value_count <= m_max_value_count && (merged_end - merged_start) <= m_max_point_distance;

    }
    range merge_ranges(const range& a, const range& b)
    {
        return {std::min(a.start_point, b.start_point), std::max(a.end_point, b.end_point), static_cast<std::uint32_t>(a.value_count + b.value_count)};
    }
public:
    range_set(std::uint32_t  max_value_count, std::uint32_t max_point_distance) : m_max_value_count(max_value_count), m_max_point_distance(max_point_distance) {}

    void add_point(std::uint32_t p)
    {
        m_ranges.push_back({p, p, 1});
    }
    void add_range(range r)
    {
        if (r.end_point < r.start_point)
        {
            return;
        }
        if (r.value_count == 0)
        {
            r.value_count = r.end_point - r.start_point + 1;
        }
        m_ranges.push_back(r);
    }
    bool contains(std::uint32_t p)
    {
        for (const auto& r : m_ranges)
        {
            if (p >= r.start_point && p <= r.end_point)
            {
                return true;
            }
        }
        return false;
    }

    std::vector<range> get_ranges()
    {
        if (m_ranges.empty())
        {
            return{};
        }
        std::vector<range> sort{m_ranges};
        std::sort(sort.begin(), sort.end(), compare_ranges);
        std::vector<range> result{};
        result.push_back(sort[0]);

        for (std::size_t i{1}; i < sort.size(); ++i)
        {
            if (can_merge(result.back(), sort[i]))
            {
                result.back() = merge_ranges(result.back(), sort[i]);
            }
            else
            {
                result.push_back(sort[i]);
            }
        }
        return result;

    }


};

void generate_points_file()
{
    std::ofstream points("points.txt");
    if (!points)
    {
        std::cerr << "Could not create\n";
        return;
    }
    std::random_device rd{};
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::uint32_t> dist(0, (1 << 30) - 1);

    for (int i = 0; i < 1'000'000; ++i)
    {
        points << dist(gen) << '\n';
    }
    std::cout << "points.txt\n" << "Generation Complete\n";
}
void generate_ranges_file()
{
    std::ofstream ranges("ranges.txt");
    if (!ranges)
    {
        std::cerr << "Could not create " << "ranges.txt\n";
    }
    std::random_device rd{};
    std::mt19937 gen (rd());
    std::uniform_int_distribution<std::uint32_t> start_dist(0, (1 << 29) - 1);
    std::uniform_int_distribution<std::uint32_t> length_dist(128, 1024);

    for (int i{0}; i < 10'000; ++i)
    {
        std::uint32_t a{start_dist(gen)};
        std::uint32_t b{a + length_dist(gen)};
        ranges << a << ":" << b << "\n";
    }
    std::cout << "ranges.txt\n" << "Generation Complete\n";
}

void load_points(range_set& rs)
{
    std::ifstream points("points.txt");
    if (!points)
    {
        std::cerr << "Could not open\n";
        return;
    }
    std::uint32_t p{};
    while (points >> p)
    {
        rs.add_point(p);
    }
}
void load_ranges(range_set& rs)
{
    std::ifstream ranges("ranges.txt");
    if (!ranges)
    {
        std::cerr << "Could not open\n";
        return;
    }
    std::string line{};
    while (std::getline(ranges, line))
    {
        if (line.empty())
        {
            continue;
        }
        std::size_t colon = line.find(':');
        if (colon == std::string::npos)
        {
            continue;
        }
        std::uint32_t start{static_cast<std::uint32_t>(std::stoul(line.substr(0, colon)))};
        std::uint32_t end{static_cast<std::uint32_t>(std::stoul(line.substr(colon + 1)))};

        if (end < start)
        {
            continue;
        }
        rs.add_range({start, end, end - start + 1});
    }

}
void write_output(const std::vector<range>& ranges)
{
    std::ofstream output("output.txt");
    if (!output)
    {
        std::cerr << "Could not create" << "output.txt" << '\n';
        return;
    }
    for (const auto& r : ranges)
    {
        output << r.start_point << ":" << r.end_point << ":" << r.value_count << '\n';
    }
    std::cout << "output.txt" << "Done!\n";
}
int main(int argc, char* argv[])
{

    if (argc < 2)
    {

    }
    std::uint32_t mVC{static_cast<std::uint32_t>(std::stoul(argv[1]))};
    std::uint32_t mPD{static_cast<std::uint32_t>(std::stoul(argv[2]))};

    range_set rs{mVC, mPD};

    load_points(rs);
    load_ranges(rs);
    auto result{rs.get_ranges()};
    write_output(result);

    return 0;
}