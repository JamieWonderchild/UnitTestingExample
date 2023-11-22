//
// Created by Jamie Aronson on 23/10/2023.
//

#ifndef TESTINGPROJECT_ASYNCTESTING_HPP
#define TESTINGPROJECT_ASYNCTESTING_HPP

#include <set>
#include <sstream>
#include <array>
#include <iostream>
#include <future>
#include <vector>

struct StopWatch {
    StopWatch(std::chrono::nanoseconds &result) : result_(result), start_{std::chrono::high_resolution_clock::now()} {}

    ~StopWatch() {
        result_ = std::chrono::high_resolution_clock::now() - start_;
    }

    std::chrono::nanoseconds result_;
    const std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

template<typename T>
std::set<T> factorize(T x) {
    std::set<T> result{1};
    for (T candidate{2}; candidate < x; candidate++) {
        if (x % candidate == 0) {
            result.insert(candidate);
            x /= candidate;
            candidate = 1;
        }
    }
    return result;
}

std::string factor_task(unsigned long x) {
    std::chrono::nanoseconds elapsed_time;
    std::set<unsigned long long> factors;
    {
        StopWatch s(elapsed_time);
        factors = factorize<unsigned long long>(x);
    }
    const auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
    std::stringstream ss;
    ss << elapsed_time_ms << " ms: Factoring " << x << " ( ";
    for (const auto factor: factors) ss << factor << " ";
    ss << ")\n";
    return ss.str();
}

std::array<unsigned long long, 6> numbers{
        9'699'690,
        179'426'549,
        1'000'000'007,
        4'294'967'291,
        4'294'967'296,
        1'307'674'368'000
};

int do_factorization() {
    std::vector<std::future<std::string>> future_tasks;
    std::chrono::nanoseconds elapsed_time;
    {
        StopWatch s(elapsed_time);
        for (auto number: numbers) {
            future_tasks.emplace_back(std::async(std::launch::async, factor_task, number));
            std::cout << factor_task(number);
        }
        const auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
        std::cout << elapsed_time_ms << " ms: Total program time \n";
    }
    return -1;
}

#endif //TESTINGPROJECT_ASYNCTESTING_HPP
