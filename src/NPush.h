#ifndef NPush_h
#define NPush_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <NDefs.h>
#include "NPushEvents.h"
#include "NPushMacros.h"

#define NPUSH_MAJOR_VERSION 2
#define NPUSH_MINOR_VERSION 0
#define NPUSH_PATCH_VERSION 0

#ifdef NTimer_h
	#define NPush_TIME() runtime
	#define NPUSH_INTERVAL_CHECK() interval(lastDebounceTime, debounceDelay)
#else
	#define NPush_TIME() millis()
	#define NPUSH_INTERVAL_CHECK() millis() - lastDebounceTime >= debounceDelay
#endif

class Push
{
public:
	Push(byte, bool, int);
	~Push();
	void update();
	bool current();
	bool pushed();
	bool released();
	uint16_t getHoldTime();
	EventHandler onRelease;
	EventHandler onPush;

private:
	const byte pin;
	const uint16_t debounceDelay;
	byte sreg;
	OnReleaseEventArgs onReleaseArgs;
	OnPushEventArgs onPushArgs;
	uint32_t lastDebounceTime;
	uint16_t holdTime;
};

extern void NPush_h_userLoop();

#ifndef NPush_cpp
#define loop NPush_h_userLoop
#endif

#endif