#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

static int counter = 0;
static std::mutex m;

void f(int a)
{
    try
    {
        std::lock_guard<std::mutex> l(m);
        while (++counter <= 100000)
        {
            std::cout << "watek " << a<< " counter " << counter << std::endl;
            if (counter == 49)
            {
                throw 1;
            }
            m.unlock();
            m.lock();
        }
    } catch (int)
    {}
}

int main()
{
    std::thread t(f, 1);
    std::thread t2(f, 2);
    f(3);
    t.join();
    t2.join();
}
