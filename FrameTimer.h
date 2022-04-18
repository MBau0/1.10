#ifndef FRAMETIMER_H
#define FRAMETIMER_H

#include <stdint.h>

class FrameTimer {
public:
	FrameTimer();
	~FrameTimer();

	void update();

	float get_frame_time_ms() const;
	float get_fps()	const;
private:
	uint64_t	  _last_cycle;
	uint64_t	  _next_cycle;
	float		  _cycle_delta;
	float		  _clock_speed_mhz;

	float		  _frame_time_ms;
	float		  _frame_time_limit_ms;
	float		  _fps;
	int			  _frame_count;

	uint64_t	  _frequency;
	uint64_t	  _last_frame;
	uint64_t	  _next_frame;
};

#endif
