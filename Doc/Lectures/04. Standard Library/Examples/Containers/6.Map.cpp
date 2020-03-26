#include <iostream>
#include <map>

int main()
{
	std::map<int, int> l_map;
	l_map[1] = 5;
	l_map[2] = 7;
	l_map[8] = 4;
	l_map[1] = 2;
	std::cout << "Count: " << l_map.size() << 	std::endl;
	for (const auto& it: l_map)
	{
		std::cout << it.first << "->" << it.second << "\n";
	}
	std::cout << std::endl;

	for (const auto& [key, value]: l_map) // C++17
	{
		std::cout << key<< "->" << value << "\n";
	}

	return 0;
}
