#include <NPush.h>

/*
    This sketch is an example of using a callback function when the button gets pressed by setting
    button.onPress to a function pointer that returns void and take void as arguments.
*/

//Serial.begin() rate
int baudrate = 9600;

//Pin the button is on.
int buttonPin = 2;

//Debounce time.
int debounce = 30;

//Pin the LED is on.
int ledPin = 3;

//State the LED is.
int currentLEDState = LOW;

Push button = Push(buttonPin, INPUT_PULLUP, debounce);
//                    ^             ^          ^
//                   pin      pull up/down debounce time

void setup()
{
    Serial.begin(baudrate);

    //Setting LED Pin as OUTPUT.
    pinMode(ledPin, OUTPUT);

    //Setting LED Pin to currentLEDState, this is the starting state.
    digitalWrite(ledPin, currentLEDState);

    //Adding a function that runs when the button gets pressed.
    button.onPress = toggleLED;
}

void loop()
{
    //Read the pin, calculates when pressed/released and hold time.
    button.update();
}

//Your function that runs when the button gets pressed.
void toggleLED()
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
    Serial.print("Pressed button, setting LED to: ");
    Serial.println(currentLEDState);

    //Writing the new state to the LED pin.
    digitalWrite(ledPin, currentLEDState);
}