#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

// FIX (it prints duplicates) - maybe rewrite with algorithms?
template <typename T, typename U>
auto get_stable_intersection_indices(const T& a1, const U& a2)
{
    using std::size;
    std::vector<size_t> result;
    for (size_t i = 0u; i < size(a1); ++i)
    {
        for (size_t j = 0u; j < size(a2); ++j)
        {
            if (a1[i] == a2[j])
            {
                result.push_back(i);
            }
        }
    }
    return result;
}

template <typename I, typename T>
void print_index_value(I const& indices, T const& range)
{
    for (auto&& i : indices)
    {
        std::cout << i << ":" << range[i] << " ";
    }
    std::cout << "\n";
}

int main() {
    std::vector<int> a1{1, 11, 22, 33, 444, 1243, 12, 11};
    std::vector<int> a2{77, 89, 88, 22, 33, 1234, 12, 112, 22, 34};
    auto indices = get_stable_intersection_indices(a1, a2);
    print_index_value(indices, a1);
}




#if 0
#include <numeric>

template <typename T, typename U>
auto get_stable_intersection_indices_2(const T& a1, const U& a2)
{
    using std::begin;
    using std::end;
    using std::size;

    std::vector<size_t> result(size(a1));
    std::iota(begin(result), end(result), 0);

    auto end_range = std::remove_if(begin(result), end(result), [&](auto i1)
    {
        return not std::any_of(begin(a2), end(a2), [e1 = a1[i1]] (auto&& e2) { return e1 == e2; });
    });
    result.erase(end_range, end(result));

    return result;
}

#include <string>
#include <sstream>

template <typename I, typename T>
void print_index_value_2(I const& indices, T const& range)
{
    using std::begin;
    using std::end;

    std::transform(begin(indices), end(indices), std::ostream_iterator<std::string>(std::cout, " "),
                   [&](auto index) {
                       std::ostringstream ss;
                       ss << index << ':' << range[index];
                       return ss.str();
                   });
    std::cout << "\n";
}

int main() {
    std::vector<int> a1{1, 11, 22, 33, 444, 1243, 12, 11};
    std::vector<int> a2{77, 89, 88, 22, 33, 1234, 12, 112, 22, 34};
    auto indices = get_stable_intersection_indices_2(a1, a2);
    print_index_value_2(indices, a1);
}
#endif
