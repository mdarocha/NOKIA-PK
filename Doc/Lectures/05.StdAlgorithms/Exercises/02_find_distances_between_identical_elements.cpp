#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <forward_list>

template <typename T>
auto find_all_distances_between_indentical_elements(const T& a)
{
    using std::begin;
    using std::end;
    std::vector<size_t> result;
    for (auto i = begin(a), e = end(a); i != e; ++i)
    {
        for (auto j = i + 1; j != e; ++j)
        {
            if (*i == *j)
            {
                result.push_back(j - i);
            }
        }
    }
    return result;
}

template <typename T>
void print_all(T const& a)
{
    for (auto&& e : a)
    {
        std::cout << e << " ";
    }
    std::cout << "\n";
}

int main() {
    std::vector<int> a{1, 33, 11, 22, 22, 33, 444, 1243, 12, 22};
    // MAKE IT WORK FOR forward_list too
    // std::forward_list<int> a{1, 33, 11, 22, 22, 33, 444, 1243, 12, 22};
    auto d = find_all_distances_between_indentical_elements(a);
    print_all(d);
}




template <typename T>
auto find_all_distances_between_indentical_elements_2(const T& a)
{
    using std::begin;
    using std::end;
    std::vector<size_t> result;
    for (auto i = begin(a), e = end(a); i != e; ++i)
    {
        for (auto j = std::next(i); j != e; ++j)
        {
            if (*i == *j)
            {
                result.push_back(std::distance(i, j));
            }
        }
    }
    return result;
}

template <typename T>
auto find_all_distances_between_indentical_elements_3(const T& a)
{
    using namespace std;

    vector<size_t> result;
    for (auto i = begin(a), e = end(a); i != e; ++i)
    {
        for (auto j = find(next(i), e, *i);
                  j != e;
                  j = find(next(j), e, *i))
        {
            result.push_back(distance(i, j));
        }
    }
    return result;
}


