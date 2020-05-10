#include <iostream>
#include <thread>
#include <future>

static std::promise<int> p;

void f(int a, int b)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));

    try {
        throw(1);
    } catch (...) {
        p.set_exception(std::current_exception());

    }

    //return a + b;
}

int main()
{
    std::thread t(f, 2, 3);

    auto f = p.get_future();

    try {
    std::cout << "2+3=" << f.get() << std::endl;
    }
    catch (int)
    {

    }

    t.join();
}
