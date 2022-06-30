#include "PeriodicTimer.h"

// Use GetTickCount64 if seriously deploying or else bad things happen

PeriodicTimer::PeriodicTimer(DWORD period_ms) :
    _period_ms          ( period_ms ),
    _time_ms            ( 0 ),
    _start_time_ms      ( GetTickCount() )
{}

DWORD PeriodicTimer::time() {
    _time_ms = GetTickCount() - _start_time_ms;

    if(_time_ms >= _period_ms) {
        _time_ms = 0;
        _start_time_ms = GetTickCount();
        return -1;
    }

    return _time_ms;
}

void PeriodicTimer::offset(DWORD offset) {
    _start_time_ms -= offset;
}

bool PeriodicTimer::alert() {
    _time_ms = GetTickCount() - _start_time_ms;

    if(_time_ms>= _period_ms) {
        _time_ms = 0;
        _start_time_ms = GetTickCount();
        return true;
    }

    return false;
}