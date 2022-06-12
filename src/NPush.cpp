#define NPush_cpp
#include "NPush.h"

static uint8_t instanceCount = ZERO;
static Push **instances = NULL;

/// <summary>
/// Push Class.
/// </summary>
/// <param name="_pin">Button pin.</param>
/// <param name="_inverted">invert the input.</param>
/// <param name="_debounceDelay">refresh time for debouncing.</param>
Push::Push(byte _pin, bool _inverted, int _debounceDelay)
	:pin(_pin), inverted(_inverted), debounceDelay(_debounceDelay), onPressTime(NULL), releasedHoldTime(NULL), lastDebounceTime(NULL), pressedHoldTime(NULL), onRelease(EventHandler()) ,onPush(EventHandler()), onReleaseArgs(OnReleaseEventArgs(this)),onPushArgs(OnPushEventArgs(this))
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
	pinMode(_pin, (inverted) ? INPUT_PULLUP : INPUT);
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
		called[PRESSEDMEMBER] = false;
		called[RELEASEDMEMBER] = false;
		
		#ifndef NTimer_h
		lastDebounceTime = NPush_TIME();
		#endif

		state[CURRENT] = (inverted) ? !digitalRead(pin) : digitalRead(pin);

		if (!state[CURRENT] && state[PREVIOUS])
		{
			state[RELEASE] = true;
			state[PREVIOUS] = false;
			state[PRESS] = false;
			
			releasedHoldTime = NPush_TIME() - onPressTime;
			pressedHoldTime = ZERO;

			onReleaseArgs.pressedAt = NPush_TIME();
			onReleaseArgs.holdTime = releasedHoldTime;

			onRelease.invoke(&onReleaseArgs);
		}
		else
		{
			state[RELEASE] = false;
		}

		if (state[CURRENT] && state[PREVIOUS])
		{
			pressedHoldTime += debounceDelay;
		}

		if (state[CURRENT] && !state[PREVIOUS])
		{
			state[PREVIOUS] = true;
			state[PRESS] = true;

			releasedHoldTime = ZERO;
			pressedHoldTime = ZERO;
			onPressTime = NPush_TIME();

			onPushArgs.pressedAt = onPressTime;
			onPush.invoke(&onPushArgs);
		}
		else
		{
			state[PRESS] = false;
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
	return state[CURRENT];
}

/// <summary>
/// Check if the button was just pressed(not the current state).
/// </summary>
/// <returns>Pressed bool</returns>
bool Push::pressed()
{
	if (called[PRESSEDMEMBER])
	{
		return false;
	}
	else
	{
		called[PRESSEDMEMBER] = true;
		return state[PRESS];
	}
}

/// <summary>
/// Check if the button was released.
/// </summary>
/// <returns>Released bool</returns>
bool Push::released()
{
	if (called[RELEASEDMEMBER])
	{
		return false;
	}
	else
	{
		called[RELEASEDMEMBER] = true;
		return state[RELEASE];
	}
}

/// <summary>
/// If just released, get the time the button was held for.
/// </summary>
unsigned int Push::getReleasedHoldTime()
{
	return releasedHoldTime;
}

/// <summary>
/// If pressed, get the time the button has been held for.
/// </summary>
unsigned int Push::getPushedHoldTime()
{
	return pressedHoldTime;
}

void loop()
{
	for (uint8_t instance = ZERO; instance < instanceCount; instance++)
	{
		if (instances[instance] != NULL) instances[instance]->update();
	}
	NPush_h_userLoop();
}