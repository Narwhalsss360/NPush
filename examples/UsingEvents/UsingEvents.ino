#include <NPush.h>

#define BAUDRATE 9600

//Pins
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4
#define BUTTON4_PIN 5

#define DEBOUNCE_TIME 30

//Buttons
Push button1(BUTTON1_PIN, INPUT_PULLUP, DEBOUNCE_TIME);
Push button2(BUTTON2_PIN, INPUT_PULLUP, DEBOUNCE_TIME);
Push button3(BUTTON3_PIN, INPUT_PULLUP, DEBOUNCE_TIME);
Push button4(BUTTON4_PIN, INPUT_PULLUP, DEBOUNCE_TIME);

//Basic Event Handler Callback.
void button1Push(PushedEventArgs& args)
{
    Serial.println("Button 1 pressed at " + String((uint32_t)args.pressedAt));
}

void button1Release(ReleasedEventArgs& args)
{
    Serial.println("Button 1 release, held for " + String((uint32_t)args.holdTime));
}

void setup()
{
    Serial.begin(BAUDRATE);
    button1.push += button1Push; //Adds a callback function when button gets pressed.
    button1.release += button1Release;
}

void loop()
{
}