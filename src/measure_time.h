#ifndef __MEASURE_TIME__
#define __MEASURE_TIME__

#include <type_traits>
#include <chrono>
#include <utility>

using milliseconds = std::chrono::milliseconds;
using microseconds = std::chrono::microseconds;
using steady_clock = std::chrono::steady_clock;

template <class T>
struct is_duration : std::false_type{};

template <class Rep, class Period>
struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type{};

template <typename T>
struct measure
{
    static_assert(is_duration<T>::value, "time must be std::chrono::duration");
      
    template<typename P>
    using ElapsedTimeWithIterations = std::pair<P, int>;

    template <class F, class... Args>
    static ElapsedTimeWithIterations<T> measure_time(F &&func, Args &&... args)
    {
        const auto t0 = steady_clock::now();
        const auto result = std::forward<F>(func)(std::forward<Args>(args)...);
        const auto iteration = std::get<1>(result); // number of Kmeans iteration
        return std::make_pair(std::chrono::duration_cast<T>(steady_clock::now() - t0), iteration);
    }
};

#endif // __MEASURE_TIME__