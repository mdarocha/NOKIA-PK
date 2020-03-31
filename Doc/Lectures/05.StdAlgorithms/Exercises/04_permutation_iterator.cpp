#include <algorithm>
#include <numeric>
#include <iostream>
#include <iterator>
#include <vector>
#include <initializer_list>
#include <type_traits>


constexpr struct end_permutation_t {} end_permutation;
template <typename T, typename UnderlyingContainer = std::vector<T>>
class permutation_iterator
{
public:

    using difference_type = long long;
    using value_type = UnderlyingContainer;
    using reference = value_type const&;
    using pointer = value_type const*;

    // EXERCICE: make it biderectional_iterator
    // QUESTION1: can we make it random_access_iterator?
    // QUESTION2: can we make it random_access_iterator?
    using iterator_category = std::forward_iterator_tag;


    template <typename InitialRange>
    permutation_iterator(InitialRange const& initial)
        : content(initial),
          first_again(false)
    {}

    // End iter
    template <typename InitialRange>
    permutation_iterator(InitialRange const& initial, end_permutation_t)
        : content(initial),
          first_again(true)
    {}


    bool operator == (const permutation_iterator& other) const
    {
        return first_again == other.first_again &&
               content == other.content;
    }
    bool operator != (const permutation_iterator& other) const
    {
        return !(*this == other);
    }

    permutation_iterator& operator ++()
    {
        using std::begin;
        using std::end;
        first_again = not std::next_permutation(begin(content), end(content));
        return *this;
    }

    permutation_iterator operator ++(int)
    {
        auto tmp = *this;
        ++tmp;
        return tmp;
    }

    reference operator *() const
    {
        return content;
    }

    pointer operator ->() const
    {
        return &content;
    }

private:
    value_type content;
    bool first_again;
};


template <typename T = int, typename UnderlyingContainer = std::vector<T>>
auto permutation_range(size_t N)
{
    using std::begin;
    using std::end;

    UnderlyingContainer initial(N);
    std::iota(begin(initial), end(initial), 0);
    return std::make_pair(permutation_iterator<T, UnderlyingContainer>(initial),
                          permutation_iterator<T, UnderlyingContainer>(initial, end_permutation));
}


int main() {
    for (auto [it, end_it] = permutation_range(4); it != end_it; ++it)
    {
        std::copy(it->begin(), it->end(), std::ostream_iterator<int>(std::cout, ","));
        std::cout << "}\n";
    }
    // cannot be compiled (with D_GLIBCXX_CONCEPT_CHECKS on gcc)
    // auto it = std::prev(permutation_range(4).first);
}


