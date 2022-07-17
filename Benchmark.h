#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <iostream>

class Benchmark {
public:
    Benchmark() :
        _start(std::chrono::high_resolution_clock::now())
    {}

    ~Benchmark() {
        stop();
    }

    void stop() {
        auto end = std::chrono::high_resolution_clock::now();

        auto s = std::chrono::time_point_cast<std::chrono::microseconds>(_start).time_since_epoch().count();
        auto e = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();

        auto d = e - s;
        double ms = d * 0.001;
        std::cout << d << "us (" << ms << "ms)\n";
    }


    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};

#endif
