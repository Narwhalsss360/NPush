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
#define NPUSH_MINOR_VERSION 1
#define NPUSH_PATCH_VERSION 0

#ifdef NTimer_h
	#define NPush_TIME() runtime
	#define NPUSH_INTERVAL_CHECK() interval(lastDebounceTime, debounceDelay)
#else
	#define NPush_TIME() millis()
	#define NPUSH_INTERVAL_CHECK() millis() - lastDebounceTime >= debounceDelay
#endif

typedef class Push;
uint8_t PushInstanceCount = ZERO;
Push **PushInstances = NULL;

enum SREGBITS
{
	CURRENT,
	PUSHED,
	RELEASED,
	PREVIOUS,
	M_PUSHED,
	M_RELEASED,
	INVERTED
};

class Push
{
public:
	Push(byte _pin, bool _inverted, int _debounceDelay)
		:pin(_pin), debounceDelay(_debounceDelay), onRelease(EventHandler()) ,onPush(EventHandler()), onReleaseArgs(), onPushArgs()
	{
		if (PushInstanceCount == ZERO)
		{
			PushInstances = new Push * [PushInstanceCount + 1];
			PushInstances[PushInstanceCount] = this;
			PushInstanceCount++;
		}
		else
		{
			Push** temp = new Push *[PushInstanceCount];
			memmove(temp, PushInstances, sizeof(Push) * PushInstanceCount);

			PushInstances = new Push *[PushInstanceCount + 1];
			memmove(PushInstances, temp, sizeof(Push) * PushInstanceCount);

			PushInstances[PushInstanceCount] = this;
			PushInstanceCount++;

			delete[] temp;
		}

		bitWrite(sreg, INVERTED, 1);
		pinMode(_pin, (_inverted) ? INPUT_PULLUP : INPUT);
		onReleaseArgs.sender = this;
		onPushArgs.sender = this;
	}

	~Push()
	{
		for (uint8_t instance = ZERO; instance < PushInstanceCount; instance++)
		{
			if (PushInstances[instance] == this)
			{
				PushInstances[instance] = NULL;
			}
		}
	}

	void update()
	{
		if (NPUSH_INTERVAL_CHECK())
		{
			bitClear(sreg, M_PUSHED);
			bitClear(sreg, M_RELEASED);
			
			#ifndef NTimer_h
			lastDebounceTime = NPush_TIME();
			#endif

			bitWrite(sreg, CURRENT, ((bitRead(sreg, INVERTED)) ? !digitalRead(pin) : digitalRead(pin)));

			if (!bitRead(sreg, CURRENT) && bitRead(sreg, PREVIOUS))
			{
				bitSet(sreg, RELEASED);
				bitClear(sreg, PREVIOUS);
				bitClear(sreg, PUSHED);
				
				onReleaseArgs.holdTime = NPush_TIME() - onReleaseArgs.pressedAt;
				onRelease.invoke(&onReleaseArgs);
			}
			else
			{
				bitClear(sreg, RELEASED);
			}

			if (bitRead(sreg, CURRENT) && bitRead(sreg, PREVIOUS))
			{
				onReleaseArgs.holdTime = NPush_TIME() - onReleaseArgs.pressedAt;
			}

			if (bitRead(sreg, CURRENT) && !bitRead(sreg, PREVIOUS))
			{
				bitSet(sreg, PREVIOUS);
				bitSet(sreg, PUSHED);

				onReleaseArgs.pressedAt = NPush_TIME();
				onPushArgs.pressedAt = onReleaseArgs.pressedAt;
				onPush.invoke(&onPushArgs);
			}
			else
			{
				bitClear(sreg, PUSHED);
			}
		}
	}

	bool current()
	{
		update();
		return bitRead(sreg, CURRENT);
	}

	bool pushed()
	{
		if (bitRead(sreg, M_PUSHED))
		{
			return false;
		}
		else
		{
			bitSet(sreg, M_PUSHED);
			return bitRead(sreg, PUSHED);
		}
	}

	bool released()
	{
		if (bitRead(sreg, M_RELEASED))
		{
			return false;
		}
		else
		{
			bitSet(sreg, M_RELEASED);
			return bitRead(sreg, RELEASED);
		}
	}

	uint16_t getHoldTime()
	{
		return onReleaseArgs.holdTime;
	}

	event onRelease;
	event onPush;

private:
	const byte pin;
	const uint16_t debounceDelay;
	byte sreg;
	OnReleaseEventArgs onReleaseArgs;
	OnPushEventArgs onPushArgs;
	uint32_t lastDebounceTime;
};

extern void NPush_h_userLoop();

void loop()
{
	for (uint8_t instance = ZERO; instance < PushInstanceCount; instance++)
	{
		if (PushInstances[instance] != NULL) PushInstances[instance]->update();
	}
	NPush_h_userLoop();
}

#define loop NPush_h_userLoop

#endif