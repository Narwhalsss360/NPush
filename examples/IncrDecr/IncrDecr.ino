#include <LiteTimer.h> //NTimer Library
#include <NPush.h>

#define BAUDRATE 9600

#define INCRDECR_INTERVAL 120

#define INCR_BUTTON_PIN 3
#define DECR_BUTTON_PIN 4
#define BUTTON_DEBOUNCE 30

Push incrementButton(INCR_BUTTON_PIN, INPUT_PULLUP, BUTTON_DEBOUNCE);
Push decrementButton(DECR_BUTTON_PIN, INPUT_PULLUP, BUTTON_DEBOUNCE);

uint32_t lastChange = 0;

int32_t counter = 0;

void setup()
{
    Serial.begin(BAUDRATE);
}

void loop()
{
    while (incrementButton.current())
    {
        if (interval(lastChange, INCRDECR_INTERVAL))
        {
            counter++;
            Serial.println(counter);
        }
    }

    while (decrementButton.current())
    {
        if (interval(lastChange, INCRDECR_INTERVAL))
        {
            counter--;
            Serial.println(counter);
        }
    }
}