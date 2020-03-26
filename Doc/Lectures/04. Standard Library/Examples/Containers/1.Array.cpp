#include <iostream>
#include <array>

int main()
{
	std::array<int, 3> arr1 = {1, 2, 3};
	std::array arr2 = {1, 2, 3}; // introduced in C++17

	arr1[0] = arr2[2] = 8; 

	std::cout << "Count: " << arr1.size() << ", " << arr2.size() << std::endl;

	for(int it : arr1)
	{
		std::cout << it << " " ;
    }

    std::cout << std::endl;
	auto [e1, e2, e3] = arr2; // introduced in C++17
	std::cout << e1 << " " << e2 << " " << e3;
}
