#include <iostream>
#include <deque>

int main()
{
	std::deque<int> deq{1,2,3};
	deq.push_back(4);
	deq.push_front(0);
	std::cout << "Count: " << deq.size() << std::endl;

	for (int it: deq)
	{
		std::cout << it << " ";
	}
}
