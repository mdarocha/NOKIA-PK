#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <forward_list>
#include <map>
#include <cassert>
#include <type_traits>


namespace my_std
{

template <class Container, typename Predicate>
auto erase_if(Container& container, Predicate&& predicate)
{
    using std::begin;
    using std::end;
    container.erase(std::remove_if(begin(container), end(container), std::forward<Predicate>(predicate)),
                    end(container));
}

template <class ...T, typename Predicate>
auto erase_if(std::forward_list<T...>& container, Predicate&& predicate)
{
    // IMPLEMENT
}

template <class ...T, typename Predicate>
auto erase_if(std::map<T...>& container, Predicate predicate)
{
    // IMPLEMENT
}

}


int main() {
    std::vector<int> a{1, 33, 11, 22, 22, 33, 444, 1243, 12, 22};
    my_std::erase_if(a, [](auto elem) { return elem % 2 == 0; });
    assert((a == std::vector{1, 33, 11, 33, 1243}));

    std::forward_list<int> b{1, 33, 11, 22, 22, 33, 444, 1243, 12, 22};
    my_std::erase_if(b, [](auto elem) { return elem % 2 == 0; });
    assert((b == std::forward_list{1, 33, 11, 33, 1243}));

    std::map<int, int> c{{1, 2}, {33, 34}, {11, 12}, {22, 23}, {22, 23}, {33, 34}, {444, 445}, {1243, 1244}, {12, 13}, {22, 23}};
    my_std::erase_if(c, [](auto const& elem) { return elem.first % 2 == 0; });
    assert((c == std::map<int,int>{{1, 2}, {33, 34}, {11, 12}, {33, 34}, {1243, 1244}}));

    std::cout << "Passed all tests!\n";
}


