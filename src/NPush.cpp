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
		this->called[PRESSEDMEMBER] = false;
		this->called[RELEASEDMEMBER] = false;
		this->lastDebounceTime = millis();
		this->state[CURRENT] = (inverted) ? !(bool)digitalRead(this->pin) : digitalRead(this->pin);

		if (!this->state[CURRENT] && this->state[PREVIOUS])
		{
			this->state[RELEASE] = true;
			this->state[PREVIOUS] = false;
			this->state[PRESS] = false;
			this->releasedHoldTime = millis() - this->onPressTime;
			this->pressedHoldTime = 0;
			if (this->onRelease != NULL) this->onRelease(this->releasedHoldTime);
		}
		else
		{
			this->state[RELEASE] = false;
		}

		if (this->state[CURRENT] && !this->state[PREVIOUS])
		{
			this->state[PREVIOUS] = true;
			this->state[PRESS] = true;
			this->releasedHoldTime = 0;
			this->pressedHoldTime = millis();
			this->onPressTime = millis();
			if (this->onPress != NULL) this->onPress();
		}
		else
		{
			this->state[PRESS] = false;
		}
	}
}

/// <summary>
/// Check the current state of the button.
/// </summary>
/// <returns>Current button state</returns>
bool Push::current()
{
	return this->state[CURRENT];
}

/// <summary>
/// Check if the button was just pressed(not the current state).
/// </summary>
/// <returns>Pressed bool</returns>
bool Push::pressed()
{
	if (this->called[PRESSEDMEMBER])
	{
		return false;
	}
	else
	{
		this->called[PRESSEDMEMBER] = true;
		return this->state[PRESS];
	}
}

/// <summary>
/// Check if the button was released.
/// </summary>
/// <returns>Released bool</returns>
bool Push::released()
{
	if (this->called[RELEASEDMEMBER])
	{
		return false;
	}
	else
	{
		this->called[RELEASEDMEMBER] = true;
		return this->state[RELEASE];
	}
}

/// <summary>
/// If just released, get the time the button was held for.
/// </summary>
unsigned int Push::getReleasedHoldTime()
{
	return this->releasedHoldTime;
}

/// <summary>
/// If pressed, get the time the button has been held for.
/// </summary>
unsigned int Push::getPushedHoldTime()
{
	return this->pressedHoldTime;
}