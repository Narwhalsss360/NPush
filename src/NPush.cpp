#define NPush_cpp
#include "NPush.h"

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

static uint8_t instanceCount = ZERO;
static Push **instances = NULL;

/// <summary>
/// Push Class.
/// </summary>
/// <param name="_pin">Button pin.</param>
/// <param name="_inverted">invert the input.</param>
/// <param name="_debounceDelay">refresh time for debouncing.</param>
Push::Push(byte _pin, bool _inverted, int _debounceDelay)
	:pin(_pin), debounceDelay(_debounceDelay), onRelease(EventHandler()) ,onPush(EventHandler()), onReleaseArgs(), onPushArgs(), holdTime(NULL)
{
	if (instanceCount == ZERO)
	{
		instances = new Push * [instanceCount + 1];
		instances[instanceCount] = this;
		instanceCount++;
	}
	else
	{
		Push** temp = new Push * [instanceCount];
		memmove(temp, instances, sizeof(Push) * instanceCount);

		instances = new Push * [instanceCount + 1];
		memmove(instances, temp, sizeof(Push) * instanceCount);

		instances[instanceCount] = this;
		instanceCount++;

		delete[] temp;
	}

	bitWrite(sreg, INVERTED, _inverted);
	pinMode(_pin, (_inverted) ? INPUT_PULLUP : INPUT);

	onReleaseArgs.sender = this;
	onPushArgs.sender = this;
}

//Does not decrement counter, should never get to this point anyways, maybe in the future it would be fixed.
Push::~Push()
{
	for (uint8_t instance = ZERO; instance < instanceCount; instance++)
	{
		if (instances[instance] == this)
		{
			instances[instance] = NULL;
		}
	}
}

/// <summary>
/// Get state, and check for if currently pressing, pressed, or released.
/// </summary>
void Push::update()
{
	if (NPUSH_INTERVAL_CHECK())
	{
		bitClear(sreg, M_PUSHED);
		bitClear(sreg, M_RELEASED);
		
		#ifndef NTimer_h
		lastDebounceTime = NPush_TIME();
		#endif

		bitWrite(sreg, CURRENT, (bitRead(sreg, INVERTED)) ? !digitalRead(pin) : digitalRead(pin)); 

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

/// <summary>
/// Check the current state of the button.
/// </summary>
/// <returns>Current button state</returns>
bool Push::current()
{
	update();
	return bitRead(sreg, CURRENT);
}

/// <summary>
/// Check if the button was just pressed(not the current state).
/// </summary>
/// <returns>Pressed bool</returns>
bool Push::pushed()
{
	if (bitRead(sreg, M_PUSHED))
	{
		return false;
	}
	else
	{
		bitSet(sreg, M_PUSHED);
		return bitRead(sreg, M_PUSHED);
	}
}

/// <summary>
/// Check if the button was released.
/// </summary>
/// <returns>Released bool</returns>
bool Push::released()
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

/// <summary>
/// If just released, get the time the button was held for.
/// </summary>
uint16_t Push::getHoldTime()
{
	return holdTime;
}

void loop()
{
	for (uint8_t instance = ZERO; instance < instanceCount; instance++)
	{
		if (instances[instance] != NULL) instances[instance]->update();
	}
	NPush_h_userLoop();
}