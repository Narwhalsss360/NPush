#include <NPush.h>

/*
    This sketch example shows how to use the Narwhalss Push Class.
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
int debounce = 30;

Push button = Push(buttonPin, INPUT_PULLUP, debounce);
//                    ^             ^          ^
//                   pin      pull up/down debounce time

void setup()
{
    Serial.begin(baudrate);
}

void loop()
{
    //Read the pin, calculates when pressed/released and hold time.
    button.update();

    //Returns true when the button gets pressed.
    if (button.pressed())
    {
        Serial.println("Pressed button.");
    }

    //Returns true while the button is pressed.
    if (button.current())
    {
        //Returns how much time you have been holding the button while pressing it.
        int holdTime = button.getPushedHoldTime();

        Serial.print("Pressing button for ");
        Serial.print(holdTime);
        Serial.println("ms.");
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