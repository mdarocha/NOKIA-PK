#include <iostream>
#include <queue>

int main()
{
	std::queue<int> l_queue;
	l_queue.push(3);
	l_queue.push(5);
	l_queue.push(1);
	std::cout << "Count: " << l_queue.size() << std::endl;
	while (!l_queue.empty())
	{
		std::cout << l_queue.front() << " ";
		l_queue.pop();
	}

	return 0;
}
