#include <iostream>
#include <thread>
#include <mutex>


static int count = 0;

void f(std::mutex& m1, std::mutex& m2)
{
    while (count <100000)
    {
        //std::lock(m1, m2);
        std::lock_guard<std::mutex> l1(m1);
        std::lock_guard<std::mutex> l2(m2);
        std::cout << "Thread " << std::this_thread::get_id() << " " << count ++ << std::endl;
    }
}

int main()
{
    std::mutex m1, m2;
    std::thread t1(f, std::ref(m1), std::ref(m2));
    std::thread t2(f, std::ref(m2), std::ref(m1));

    t1.join();
    t2.join();
}
