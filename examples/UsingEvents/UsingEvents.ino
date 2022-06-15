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
void button1Push(EventArgs *args)
{
    OnPushEventArgs data = *(OnPushEventArgs*)args;
    Serial.println("Button 1 pressed at :" + String(data.pressedAt));
}

//Basic Event Handler Callback that takes a name, and the name of the args. Same as function above
EVENT_FUNCTION(button2Push, args)
{
    OnPushEventArgs data = reinterpret_c_style(OnPushEventArgs, args);
    Serial.println("Button 2 pushed at :" + String(data.pressedAt));
}

//Basic Event Handler Callback that takes a name, and the name of the args. Same as function above but takes a body as last argument.
ESR(button3Push, args,
{
    OnPushEventArgs data = GET_OnPushEventArgs(args);
    Serial.println("Button 3 pushed at :" + String(data.pressedAt));
})

//OnPushEvent Callback that takes a name, and the name of the args. Same as function above but takes a body as last argument. This is written by library.
ONPUSH_ESR(button4Push, data,
{
    Serial.println("Button 4 pushed at :" + String(data.pressedAt));
})

//Usage is the same for onRelease, just changing datatype.
ONRELEASE_ESR(button1Release, data,
{
    Serial.println("Button 1 released, held for:" + String(data.holdTime));
})

void setup()
{
    Serial.begin(BAUDRATE);
    button1.onPush.addCallback(button1Push); //Adds a callback function when button gets pressed.
    button2.onPush += button2Push; //Same as above just using operator.
    button3.onPush += button3Push;
    button4.onPush += button4Push;
    
    button1.onRelease += button1Release;
}

void loop()
{
    while (button1.current())
    {
        Serial.print("Button 1 is being pressed and has been held for: ");
        Serial.println(button1.getHoldTime());

        //Update other buttons because while() loop is blocking. Or don't if thats what you want.
        button2.update();
        button3.update();
        button4.update();
    }
}