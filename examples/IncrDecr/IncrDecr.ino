#include <TimedInterval.h.h> //NTimer Library
#include <NPush.h>

#define BAUDRATE 9600

#define INCRDECR_INTERVAL 120

#define INCR_BUTTON_PIN 3
#define DECR_BUTTON_PIN 4
#define BUTTON_DEBOUNCE 30_ms

#define DO_NOT_BIND false //Does not use SketchBinder since current() is used and does not require it

Push incrementButton(INCR_BUTTON_PIN, INPUT_PULLUP, BUTTON_DEBOUNCE, DO_NOT_BIND);
Push decrementButton(DECR_BUTTON_PIN, INPUT_PULLUP, BUTTON_DEBOUNCE, DO_NOT_BIND);

ntime_t lastChange = 0;

int32_t counter = 0;

void setup()
{
    Serial.begin(BAUDRATE);
}

void loop()
{
    while (incrementButton.current())
    {
        if (intervalElapsed(lastChange, INCRDECR_INTERVAL))
        {
            counter++;
            Serial.println(counter);
        }
    }

    while (decrementButton.current())
    {
        if (intervalElapsed(lastChange, INCRDECR_INTERVAL))
        {
            counter--;
            Serial.println(counter);
        }
    }
}