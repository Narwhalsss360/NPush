#ifndef NPushEvents_h
#define NPushEvents_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <NEvents.h>

struct OnReleaseEventArgs : public EventArgs
{
    uint16_t holdTime;
    uint32_t pressedAt;
};

struct OnPushEventArgs : public EventArgs
{
    uint32_t pressedAt;
};

#endif