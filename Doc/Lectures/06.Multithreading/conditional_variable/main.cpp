#include <condition_variable>
#include <iostream>
#include <thread>
#include <mutex>

static int turn = 0;
static std::mutex m;
static std::condition_variable c;

void f(int a)
{
    std::unique_lock<std::mutex> l(m);
    while (turn < 20)
    {
        c.wait(l, [a](){return turn%3 == a;});
        std::cout << "Thread " << a << " turn " << turn++ << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        c.notify_all();
    }
}

int main()
{
    std::thread t1(f, 0);
    std::thread t2(f, 1);
    std::thread t3(f, 2);
    t1.join();
    t2.join();
    t3.join();
}
