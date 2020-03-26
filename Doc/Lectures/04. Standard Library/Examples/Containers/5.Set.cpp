#include <iostream>
#include <set>

int main()
{
	std::set<int> l_set;
	l_set.insert(3);
	l_set.insert(122);
	l_set.insert(2); 
	std::cout << "Count: " << l_set.size() << std::endl;

	for (int it: l_set)
	{
		std::cout << it << " " ;
	}

	return 0;
}
