#include <iostream> 

struct A
{
    int m_name{0};
    A(int p_name) : m_name(p_name) { std::cout << "A(" << m_name << ") constructed successfully\n"; }
    ~A() { std::cout << "A(" << m_name << ") destroyed\n"; }
};
 
struct B
{
    A* a1 = new A{5};
    B() { std::cout << "B constructed successfully\n"; } 
    ~B() { std::cout << "B destroyed\n"; }
};


int main() 
{
    B b();
    return 0;
}

struct BRaii\\\
{
    A* a1;
    BRaii() { 	
				a1 = new A{5};	
				std::cout << "BRaii constructed successfully\n"; 
			} 
			
    ~BRaii(){ 
				delete a1;
				std::cout << "BRaii destroyed\n"; 
			}
};