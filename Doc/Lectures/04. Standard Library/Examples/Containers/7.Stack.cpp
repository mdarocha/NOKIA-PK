#include <iostream>
#include <stack>

int main()
{
	std::stack<int> l_stack;
	l_stack.push(1);
	l_stack.push(2);
	l_stack.push(3);
	std::cout << "Count: " << l_stack.size() << std::endl;
	while (!l_stack.empty())
	{
		std::cout << l_stack.top() << " ";
		l_stack.pop();
	}

	return 0;
}
