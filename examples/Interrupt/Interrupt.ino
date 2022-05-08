#include <NPush.h>

/*
    This sketch example shows how to use the Narwhalss Push Class with interrupts.
    The Push class is used for push buttons and can check when you press the button,
    release the button, while you're pressing the button, and how long you held the
    button or how long you are currently holdig the button. This class also uses 
    debouncing for accurate reading.
*/

//Serial.begin() rate
int baudrate = 9600;

//Pin the button is on.
int buttonPin = 2;

//Debounce time.
int debounce = 10;

Push button = Push(buttonPin, INPUT_PULLUP, debounce);
//                    ^             ^          ^
//                   pin      pull up/down debounce time

void buttonISR()
{
    button.update();

    //Returns true when the button gets pressed.
    if (button.pressed())
    {
        Serial.println("Pressed button.");
    }

    //Returns true when the button gets released.
    if (button.released())
    {
        //Returns how much time in ms the button was held for. Must be called before 
        //next button.update() cycle or else the returned value will be 0.
        int holdTime = button.getReleasedHoldTime();

        Serial.print("Released button. Held for ");
        Serial.print(holdTime);
        Serial.println("ms.");
    }
}

void setup()
{
    Serial.begin(baudrate);
    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, CHANGE);
}

void loop()
{
    //Doing nothing important...
}