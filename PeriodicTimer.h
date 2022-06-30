#ifndef PERIODICTIMER_H
#define PERIODICTIMER_H

#include <Windows.h>

class PeriodicTimer {
public:
    PeriodicTimer(DWORD period_ms);

    DWORD time();

    void offset(DWORD offset);

    bool alert();
private:
    DWORD _period_ms;
    DWORD _time_ms;
    DWORD _start_time_ms;
};

#endif