#include <iostream>
#include <tuple>

int main()
{
	auto l_tuple1 = std::make_tuple(1, 2.3, "Lukasz");
	std::tuple<int, double, std::string> l_tuple2(1, 3.14, "PI");
	
	std::size_t s = std::tuple_size<decltype(l_tuple1)>::value;
	std::cout << "Size: " << s << std::endl;

	std::cout << "Tuple1: " <<
    std::get<0>(l_tuple1) << ", " <<
    std::get<1>(l_tuple1) << ", " <<
    std::get<2>(l_tuple1) << std::endl;

	std::tuple<int, float, char*> l_tuple3(1, 2.0f, nullptr);
	std::get<2>(l_tuple3) = new char[13];

	// C++17
	std::tuple l_tuple4{ 2, 6.28, "2*PI" };
	auto [index, value, name] = l_tuple4;
	std::cout << "Tuple4: " << index << ", " << value << ", " << name << std::endl;
	std::cout << (l_tuple1 == l_tuple2);
}
