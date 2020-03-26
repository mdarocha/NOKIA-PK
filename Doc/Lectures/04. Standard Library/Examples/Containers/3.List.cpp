#include <iostream>
#include <list>
#include <algorithm>

int main()
{
	std::list<int> l_list;
	l_list.push_back(3);
	l_list.push_back(2);
	l_list.push_back(1);
	std::cout << "Count: " << l_list.size() << 	std::endl;

	for (int it: l_list)
	{
		std::cout << it << " ";
	}

	auto it = std::find(l_list.begin(), l_list.end(), 2);
	if (it != l_list.end())
	{
		l_list.insert(it, 4);
	}

	std::cout << std::endl;
	for (int it: l_list)
	{
		std::cout << it << " ";
	}
}
