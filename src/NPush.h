#ifndef NPush_h
#define NPush_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <NDefs.h>

#define NPUSH_MAJOR_VERSION 1
#define NPUSH_MINOR_VERSION 0
#define NPUSH_PATCH_VERSION 2

class Push
{
public:
	Push(byte, bool, int);
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
#endif