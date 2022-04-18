#include "FrameTimer.h"

#include <Windows.h>

FrameTimer::FrameTimer() :
	_last_cycle				( __rdtsc() ),
	_next_cycle				( __rdtsc() ),
	_cycle_delta			( -1.0f ),
	_clock_speed_mhz		( -1.0f ),
	_frame_time_ms			( -1.0f ),
	_frame_time_limit_ms	( 1000.0f / 60.0f ),
	_fps					( -1.0f ),
	_frame_count			( 0 )
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&_last_frame);
	QueryPerformanceCounter((LARGE_INTEGER*)&_next_frame);
}

FrameTimer::~FrameTimer()
{}

void FrameTimer::update() {
	_next_cycle = __rdtsc();
	_cycle_delta = (float)(_next_cycle - _last_cycle);
	_last_cycle = _next_cycle;

	QueryPerformanceCounter((LARGE_INTEGER*)&_next_frame);
	_frame_time_ms = (1000.0f * (float)(_next_frame - _last_frame)) / (float)_frequency;
	_fps = _frequency / (float)(_next_frame - _last_frame);
	_last_frame = _next_frame;
}

float FrameTimer::get_frame_time_ms() const {
	return _frame_time_ms;
}

float FrameTimer::get_fps() const {
	return _fps;
}