#ifndef PERIODICTIMER_H
#define PERIODICTIMER_H

typedef unsigned long DWORD;

class PeriodicTimer {
public:
    PeriodicTimer(DWORD period_ms);

    DWORD time();

    void offset(DWORD offset);

    const DWORD remaining() const;

    const bool alert();
private:
    DWORD _period_ms;
    DWORD _start_time_ms;
};

#endif