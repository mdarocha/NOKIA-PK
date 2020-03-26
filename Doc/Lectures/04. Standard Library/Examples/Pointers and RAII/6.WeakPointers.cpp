#include <iostream>
#include <memory>

struct A
{
		int m_name;
		A(int p_name) : m_name(p_name) { std::cout << "A created\n"; }
		~A() { std::cout << "A deleted\n"; }
};

int main()
{
	std::shared_ptr<A> ptr1(std::make_shared<A>(13));
	std::shared_ptr<A> ptr2(ptr1);
	std::cout << "Number of references: " << ptr2.use_count() << std::endl;
	
	std::weak_ptr<A> wptr(ptr1);
	std::cout << "Number of references after weak_ptr is created: " << ptr2.use_count() << std::endl;
	
	ptr1.reset();
	std::cout << "Number of references: " << ptr2.use_count() << std::endl;

	{
		auto unknownTypeOfPtr = wptr.lock();
		std::cout << "Finding type of pointer\n";
		std::cout << "Number of references: " << ptr2.use_count() << std::endl;
	}
	
	ptr2.reset();
	std::cout << "Number of references: " << ptr2.use_count() << std::endl;

	{
		auto unknownValueOfPtr = wptr.lock();
		if (unknownValueOfPtr == nullptr) std::cout << "Nullptr\n";
	}
}



int main()
{
	std::shared_ptr<int> ptr1(std::make_shared<int>(13));
	std::shared_ptr<int> ptr2(ptr1); // refcnt=2
	std::weak_ptr<int> wptr(ptr1);   // still refcnt=2
	ptr1.reset();  // refcnt=1, no delete
	{ wptr.lock(); } // returns a non-null shared pointer
	ptr2.reset();  // refcnt=0, deletes object
	{ wptr.lock(); } // now returns a null shared pointer
}
