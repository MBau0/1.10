#include "PeriodicTimer.h"

#include <windows.h>

inline __time64_t get_time_ms() {
    FILETIME filetime;
    GetSystemTimeAsFileTime(&filetime);
    ULARGE_INTEGER ulInt;
    ulInt.LowPart = filetime.dwLowDateTime;
    ulInt.HighPart = filetime.dwHighDateTime;
    return (ulInt.QuadPart / 10000);
}

PeriodicTimer::PeriodicTimer(__time64_t period_ms) :
    _period_ms          ( period_ms ),
    _time_ms            ( 0 ),
    _start_time_ms      ( get_time_ms() )
{}

__time64_t PeriodicTimer::time() {
    __time64_t current_time_ms = get_time_ms();

    _time_ms = current_time_ms - _start_time_ms;

    if(_time_ms >= _period_ms) {
        _time_ms = 0;
        _start_time_ms = get_time_ms();
        return -1;
    }

    return _time_ms;
}

void PeriodicTimer::offset(__time64_t offset) {
    _start_time_ms -= abs(offset);
}

bool PeriodicTimer::alert() {
    __time64_t current_time_ms = get_time_ms();

    _time_ms = current_time_ms - _start_time_ms;

    if(_time_ms >= _period_ms) {
        _time_ms = 0;
        _start_time_ms = get_time_ms();
        return true;
    }

    return false;
}