#include <iostream>
#include <memory>

class B;

class A
{
    std::shared_ptr<B> sP1; 

public:
    A() { std::cout << "A()" << std::endl; }
    ~A() { std::cout << "~A()" << std::endl; }

    void setShared(std::shared_ptr<B>& p)
    {
        sP1 = p;
    }
};

class B
{
    std::shared_ptr<A> sP1;

public:
    B() { std::cout << "B()" << std::endl; }
    ~B() { std::cout << "~B()" << std::endl; }

    void setShared(std::shared_ptr<A>& p)
    {
        sP1 = p;
    }
};

int main()
{
    std::shared_ptr<A> aPtr(new A);
    std::shared_ptr<B> bPtr(new B);

    aPtr->setShared(bPtr);
    bPtr->setShared(aPtr);

    return 0;  
}


// use weak_ptr instead to avoid CD