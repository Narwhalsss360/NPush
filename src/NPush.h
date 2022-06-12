#ifndef NPush_h
#define NPush_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <NDefs.h>

#define NPUSH_MAJOR_VERSION 1
#define NPUSH_MINOR_VERSION 1
#define NPUSH_PATCH_VERSION 0

#ifdef NTimer_h
	#define NPush_TIME() runtime
#else
	#define NPush_TIME() millis()
#endif

class Push
{
public:
	Push(byte, bool, int);
	~Push();
	void update();
	bool current();
	bool pressed();
	bool released();
	unsigned int getReleasedHoldTime();
	unsigned int getPushedHoldTime();
	void (*onRelease) (unsigned int);
	void (*onPress) (void);

private:
	enum BUTTONSTATESINDEX
	{
		CURRENT,
		PRESS,
		RELEASE,
		PREVIOUS
	};
	enum CLASSMEMBERS
	{
		PRESSEDMEMBER,
		RELEASEDMEMBER
	};
	const byte pin;
	const bool inverted;
	const unsigned short debounceDelay;
	bool state[4];
	unsigned long onPressTime;
	unsigned long lastDebounceTime;
	unsigned int releasedHoldTime;
	unsigned int pressedHoldTime;
	bool called[2];
};


extern void NPush_h_userLoop();

#ifndef NPush_cpp
#define loop NPush_h_userLoop
#endif

#endif