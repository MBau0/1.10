#ifndef PERIODICTIMER_H
#define PERIODICTIMER_H

#include <time.h>

class PeriodicTimer {
public:
    PeriodicTimer(__time64_t period_ms);

    __time64_t time();

    void offset(__time64_t offset);

    void test();

    bool alert();
private:
    __time64_t _period_ms;
    __time64_t _time_ms;
    __time64_t _start_time_ms;
};

#endif