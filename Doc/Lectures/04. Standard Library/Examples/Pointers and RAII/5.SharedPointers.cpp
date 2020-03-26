#include <iostream>
#include <memory>

struct A
{
    int m_name{0};
    A(int p_name) : m_name(p_name) { std::cout << "A(" << m_name << ") constructed successfully\n"; }
    ~A() { std::cout << "A(" << m_name << ") destroyed\n"; }
	void getName() { std::cout << __FUNCTION__ << ": A(" << m_name << ")\n"; }
};

void foo(std::shared_ptr<A> p_ptr) 
{
	p_ptr->getName();
// 	std::cout << "Number of references inside a function:" << p_ptr.use_count() << std::endl;
	std::cout << "Exiting foo()\n";
}


int main()
{
	std::shared_ptr<A> ptr1(std::make_shared<A>(13));
	std::shared_ptr<A> ptr2 = ptr1;
	std::cout << "Number of references before a function:" << ptr2.use_count() << std::endl;
	foo(ptr1);
    std::cout << "Number of references after a function:" << ptr1.use_count() << std::endl;

	std::cout << "Exiting main()\n";
}




