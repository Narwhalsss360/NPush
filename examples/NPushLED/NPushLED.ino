#include <NPush.h>

/*
    This sketch is an example of using a callback function when the button gets pressed by using '+='
    to button.onPush. Use ONPUSH_ESR/ONRELEASE_ESR for easy usage.
*/

//Serial.begin() rate
int baudrate = 9600;

//Pin the button is on.
int buttonPin = 2;

//Debounce time.
int debounce = 30;

//Pin the LED is on.
int ledPin = LED_BUILTIN;

//State the LED is.
int currentLEDState = LOW;

Push button = Push(buttonPin, INPUT_PULLUP, debounce);
//                    ^             ^          ^
//                   pin      pull up/down debounce time

//Your function that runs when the button gets pressed.
void toggleLED(PushedEventArgs& args)
{
    //Toggles the LED
    if (currentLEDState == HIGH)
    {
        currentLEDState = LOW;
    }
    else
    {
        currentLEDState = HIGH;
    }

    //Print that the LED state is changing and what its changing to.
    Serial.print("Pushed button, setting LED to: ");
    Serial.println(currentLEDState);

    //Writing the new state to the LED pin.
    digitalWrite(ledPin, currentLEDState);
}

void setup()
{
    Serial.begin(baudrate);

    //Setting LED Pin as OUTPUT.
    pinMode(ledPin, OUTPUT);

    //Setting LED Pin to currentLEDState, this is the starting state.
    digitalWrite(ledPin, currentLEDState);

    //Adding a function that runs when the button gets pressed.
    button.push += toggleLED;
}

void loop()
{
}