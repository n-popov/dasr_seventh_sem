#include <numeric>
#include <iterator>
#include <thread>
#include <vector>
#include <iostream>

#define RANGE(container) std::begin(container), std::end(container)

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

// Функция-обёртка

template <typename Iterator, typename T>
void accumulate_wrapper(Iterator begin, Iterator end, T init, T& result) {
    result = std::accumulate(begin, end, init);
}

template <typename Iterator, typename T>
T parallel_accumulate(Iterator begin, Iterator end, T init) {
    auto distance = std::distance(begin, end);
    if (distance < 2 * 4) {
        return std::accumulate(begin, end, init);
    }
    auto middle = std::next(begin, distance / 2);
    T result;
    std::thread other_thread(accumulate_wrapper<Iterator, T>,
                             begin, middle, 0, std::ref(result));
    T main_result = std::accumulate(middle, end, init);
    other_thread.join();
    return result + main_result;
}

int main() {
    std::vector<int> numbers(1000000);
    std::iota(RANGE(numbers), 0);
    {
        Timer<std::chrono::microseconds> t1;
        std::cout << std::accumulate(RANGE(numbers), 0) << std::endl;
    }
    {
        Timer<std::chrono::microseconds> t2;
        std::cout << parallel_accumulate(RANGE(numbers), 0) << std::endl;
    }
}