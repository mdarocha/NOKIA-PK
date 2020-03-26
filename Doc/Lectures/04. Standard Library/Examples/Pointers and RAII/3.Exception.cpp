#include <iostream> 
#include <stdexcept>

struct A {
    int n;
    A(int n = 0): n(n) { std::cout << "A(" << n << ") constructed successfully\n"; }
    ~A() { std::cout << "A(" << n << ") destroyed\n"; }
};
 
int foo()
{
    throw std::runtime_error("Error");
}
 
struct B {
    A a1, a2, a3;
    B() try : a1(1), a2(foo()), a3(3) {
        std::cout << "B constructed successfully\n";
    } catch(...) {
    	std::cout << "B::B() exiting with exception\n";
    }
    ~B() { std::cout << "B destroyed\n"; }
};
 

int main () try
{
    B b{};
    
} catch (const std::exception& e) {
    std::cout << "main() failed to create B with: " << e.what();
    
    return 0;
}









