#include "NPush.h"

/// <summary>
/// Push Class.
/// </summary>
/// <param name="_pin">Button pin.</param>
/// <param name="_inverted">invert the input.</param>
/// <param name="_debounceDelay">refresh time for debouncing.</param>
Push::Push(byte _pin, bool _inverted, int _debounceDelay)
	:pin(_pin), inverted(_inverted), debounceDelay(_debounceDelay), onRelease(NULL), onPress(NULL), onPressTime(NULL), releasedHoldTime(NULL), lastDebounceTime(NULL), pressedHoldTime(NULL)
{
	pinMode(_pin, (inverted) ? INPUT_PULLUP : INPUT);
}

/// <summary>
/// Get state, and check for if currently pressing, pressed, or released.
/// </summary>
void Push::update()
{
	if (millis() - this->lastDebounceTime >= this->debounceDelay)
	{
		called[PRESSEDMEMBER] = false;
		called[RELEASEDMEMBER] = false;
		lastDebounceTime = millis();
		state[CURRENT] = (inverted) ? !(bool)digitalRead(pin) : digitalRead(pin);

		if (!state[CURRENT] && state[PREVIOUS])
		{
			state[RELEASE] = true;
			state[PREVIOUS] = false;
			state[PRESS] = false;
			releasedHoldTime = millis() - onPressTime;
			pressedHoldTime = 0;
			if (onRelease != NULL) onRelease(releasedHoldTime);
		}
		else
		{
			state[RELEASE] = false;
		}

		if (state[CURRENT] && !state[PREVIOUS])
		{
			state[PREVIOUS] = true;
			state[PRESS] = true;
			releasedHoldTime = 0;
			pressedHoldTime = millis();
			onPressTime = millis();
			if (onPress != NULL) onPress();
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