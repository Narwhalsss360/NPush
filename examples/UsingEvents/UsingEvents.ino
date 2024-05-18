#include <AutoBind.h>
#include <NPush.h>

#define BAUDRATE 9600

//Pins
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4
#define BUTTON4_PIN 5

#define DEBOUNCE_TIME 30

//Buttons
Push buttons[] =
{
    Push(BUTTON1_PIN, INPUT_PULLUP, DEBOUNCE_TIME),
    Push(BUTTON2_PIN, INPUT_PULLUP, DEBOUNCE_TIME),
    Push(BUTTON3_PIN, INPUT_PULLUP, DEBOUNCE_TIME),
    Push(BUTTON4_PIN, INPUT_PULLUP, DEBOUNCE_TIME)
}

size_t buttonCount = sizeof(buttons) / sizeof(buttons[0]);

void buttonPushed(PushedEventArgs& args)
{
    int index = args.sender - buttons;
    if (index < 0 || index >= buttonCount)
        return; //Pointer error

    Serial.print("Button ");
    Serial.print(index + 1);
    Serial.println(" Pushed");
}

void buttonReleased(ReleasedEventArgs& args)
{
    int index = args.sender - buttons;
    if (index < 0 || index >= buttonCount)
        return; //Pointer error

    Serial.print("Button ");
    Serial.print(index + 1);
    Serial.print(" Released, Held for ");
    Serial.println(args.holdTime.milliseconds());
}

void setup()
{
    Serial.begin(BAUDRATE);
    for (int i = 0; i < buttonCount; i++)
    {
        buttons[i].push += buttonPushed;
        buttons[i].release += buttonReleased;
    }
}

void loop()
{
}