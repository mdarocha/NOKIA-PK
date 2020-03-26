#include <iostream>
#include <vector>

int main()
{
	std::vector<int> vec;
	std::vector<int> vec1 { 1, 2, 3 };
	std::cout << "Count: " << vec.size() << std::endl;
	std::cout << "Capacity: " << vec.capacity() << std::endl;

	vec.push_back(3);
	vec.push_back(1);
	vec.push_back(2);

	std::cout << "Count: " << vec.size() << std::endl;
	std::cout << "Capacity: " << vec.capacity() << std::endl;

	for (int it: vec)
	{
		std::cout << it << " ";
	}
	std::cout << std::endl;

	for (int it: vec1)
	{
		std::cout << it << " ";
	}
	std::cout << std::endl;
	std::vector<int>::iterator it1 = vec1.begin();
	std::cout << *it1 << " ";

}
