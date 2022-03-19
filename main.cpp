#include <iostream>
#include <chrono>
#include <cmath>

template <typename Duration>
class Timer {
public:
    Timer(): begin(std::chrono::steady_clock::now()) {}
    ~Timer() {
        std::cout << "Time is " <<
        std::chrono::duration_cast<Duration>
                (std::chrono::steady_clock::now() - begin).count()
                << std::endl;
    }
private:
    std::chrono::steady_clock::time_point begin;
};

int main() {
    auto sum = 0.;
    {
        Timer<std::chrono::milliseconds> t;
        for(auto i = 0; i < 100000; i++) {
            sum += std::sin(i) + std::cos(i);
        }
    }
    std::cout << "Sum is " << sum << std::endl;
    return 0;
}
