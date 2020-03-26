#include <iostream>
#include <queue>

int main()
{
	std::priority_queue<int> pqueue;
	pqueue.push(122);
	pqueue.push(2);
	pqueue.push(33);
	std::cout << "Count: " << pqueue.size() << std::endl;
	while (!pqueue.empty()) 
	{
		std::cout << pqueue.top() << " ";
		pqueue.pop();
	}

	return 0;
}


