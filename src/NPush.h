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

#ifdef NTimer_h
	#define time() runtime
	#define NPUSH_INTERVAL_CHECK() interval(lastDebounceTime, debounceDelay)
#else
	#define time() millis()
	#define NPUSH_INTERVAL_CHECK() millis() - lastDebounceTime >= debounceDelay
#endif

typedef bool (*VirtualPushReader) (void);
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
			memmove(temp, PushInstances, sizeof(Push *) * PushInstanceCount);

			PushInstances = new Push *[PushInstanceCount + 1];
			memmove(PushInstances, temp, sizeof(Push *) * PushInstanceCount);

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
			lastDebounceTime = time();
			#endif

			bitWrite(sreg, CURRENT, ((bitRead(sreg, INVERTED)) ? !digitalRead(pin) : digitalRead(pin)));

			if (!bitRead(sreg, CURRENT) && bitRead(sreg, PREVIOUS))
			{
				bitSet(sreg, RELEASED);
				bitClear(sreg, PREVIOUS);
				bitClear(sreg, PUSHED);
				
				onReleaseArgs.holdTime = time() - onReleaseArgs.pressedAt;
				onRelease.invoke(&onReleaseArgs);
			}
			else
			{
				bitClear(sreg, RELEASED);
			}

			if (bitRead(sreg, CURRENT) && bitRead(sreg, PREVIOUS))
			{
				onReleaseArgs.holdTime = time() - onReleaseArgs.pressedAt;
			}

			if (bitRead(sreg, CURRENT) && !bitRead(sreg, PREVIOUS))
			{
				bitSet(sreg, PREVIOUS);
				bitSet(sreg, PUSHED);

				onReleaseArgs.pressedAt = time();
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

class VirtualPush
{
public:
	VirtualPush(VirtualPushReader reader, uint32_t debounceDelay)
		: onRelease(EventHandler()), onPush(EventHandler()), reader(reader), debounceDelay(debounceDelay), lastDebounceTime(0), sreg(0), onReleaseArgs(OnReleaseEventArgs()), onPushArgs(OnPushEventArgs())
	{
	}

	~VirtualPush()
	{
	}

	void update()
	{
		if (NPUSH_INTERVAL_CHECK())
		{
			bitClear(sreg, M_PUSHED);
			bitClear(sreg, M_RELEASED);
		
#ifndef NTimer_h
			lastDebounceTime = time();
#endif

			bitWrite(sreg, CURRENT, reader());

			if (!bitRead(sreg, CURRENT) && bitRead(sreg, PREVIOUS))
			{
				bitSet(sreg, RELEASED);
				bitClear(sreg, PREVIOUS);
				bitClear(sreg, PUSHED);

				onReleaseArgs.holdTime = time() - onReleaseArgs.pressedAt;
				onRelease.invoke(&onReleaseArgs);
			}
			else
			{
				bitClear(sreg, RELEASED);
			}

			if (bitRead(sreg, CURRENT) && bitRead(sreg, PREVIOUS))
			{
				onReleaseArgs.holdTime = time() - onReleaseArgs.pressedAt;
			}

			if (bitRead(sreg, CURRENT) && !bitRead(sreg, PREVIOUS))
			{
				bitSet(sreg, PREVIOUS);
				bitSet(sreg, PUSHED);

				onReleaseArgs.pressedAt = time();
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
	VirtualPushReader reader;
	uint32_t debounceDelay;
	uint32_t lastDebounceTime;
	byte sreg;
	OnReleaseEventArgs onReleaseArgs;
	OnPushEventArgs onPushArgs;
};

extern void NPush_h_userLoop();

void NPush_h_loop()
{
	for (uint8_t instance = ZERO; instance < PushInstanceCount; instance++)
	{
		if (PushInstances[instance] != NULL) PushInstances[instance]->update();
	}
}

void loop()
{
	NPush_h_loop();
	NPush_h_userLoop();
}

#define loop NPush_h_userLoop
#undef time

#endif