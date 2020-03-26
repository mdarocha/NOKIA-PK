#include <iostream>
#include <memory>

struct A
{
    int m_name;
	A() { std::cout << "A created" << std::endl;} 
    A(int p_name) : m_name(p_name) { std::cout << "A(" << m_name << ") constructed successfully\n"; } 
    ~A() { std::cout << "A(" << m_name << ") destroyed\n"; }
};

std::unique_ptr<A> foo()
{ 
	return std::make_unique<A>(1);
}

int main()
{
	std::unique_ptr<A> ptr1(std::make_unique<A>(13));
	std::unique_ptr<A> ptr2 = ptr1; 
	
	//SECTION 1
// 	std::unique_ptr<A> ptr2 = std::move(ptr1);
// 	ptr1.reset(); 
// 	ptr2.reset(); 
// 	std::unique_ptr<A> ptr3 = foo(); 

    //SECTION 2
	// A* ptr0 = new A{2};
	// std::unique_ptr<A> ptr4(ptr0);

    //SECTION 3
// 	{
// 		int* ptr1 = new int(5);
// 		std::unique_ptr<int> ptr2(ptr1); 
// 		std::unique_ptr<int> ptr3(ptr1); 
// 	}

    //SECTION 4
	//{
// 		std::unique_ptr<A[]> ptr5(std::make_unique<A[]>(3)); 
// 		std::unique_ptr<int> ptr3(ptr1); 
// 	}

	std::cout << "Exiting main()\n";
}



