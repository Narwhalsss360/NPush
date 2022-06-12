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
    OnReleaseEventArgs(void *_sender);
    OnReleaseEventArgs(uint16_t, uint32_t);
};

struct OnPushEventArgs : public EventArgs
{
    uint32_t pressedAt;
    OnPushEventArgs(void *_sender);
    OnPushEventArgs(uint32_t);
};
