#include "PeriodicTimer.h"

#include <Windows.h>

// Use GetTickCount64 if seriously deploying or else bad things happen

PeriodicTimer::PeriodicTimer(DWORD period_ms) :
    _period_ms          ( period_ms ),
    _start_time_ms      ( GetTickCount() )
{}

DWORD PeriodicTimer::time() {
    const auto time_ms = GetTickCount() - _start_time_ms;

    if(time_ms >= _period_ms) {
        _start_time_ms = GetTickCount();
        return -1;
    }

    return time_ms;
}

void PeriodicTimer::offset(DWORD offset) {
    _start_time_ms -= offset;
}

const DWORD PeriodicTimer::remaining() const {
    const auto r = _period_ms - (GetTickCount()  - _start_time_ms);
    return (r > 0) ? r : 0;
}

const bool PeriodicTimer::alert() {
    const auto time_ms = GetTickCount() - _start_time_ms;

    if(time_ms >= _period_ms) {
        _start_time_ms = GetTickCount();
        return true;
    }

    return false;
}