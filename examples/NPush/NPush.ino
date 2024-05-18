#include <AutoBind.h> //<- Auto update
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
    //button.update(); //Now done automagically, if running blocking code OR did not include AutoBind/SketchBinder, run this.

    //Returns true when the button gets pressed.
    if (button.pushed())
    {
        Serial.println("Pushed button.");
    }

    //Returns true while the button is pressed.
    if (button.current())
    {
        //Returns how much time you have been holding the button while pressing it.
        ntime_t holdTime = button.getHoldTime();

        Serial.print("Held button for ");
        Serial.print(holdTime.milliseconds());
        Serial.println("ms.");
    }

    //Returns true when the button gets released.
    if (button.released())
    {
        //Returns how much time in ms the button was held for. Must be called before 
        //next button.update() cycle or else the returned value will be 0.
        ntime_t holdTime = button.getHoldTime();

        Serial.print("Released button. Held for ");
        Serial.print(holdTime.milliseconds());
        Serial.println("ms.");
    }
}