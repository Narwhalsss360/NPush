#ifndef NPush_h
#define NPush_h

#include <Arduino.h>
#include <NEvents.h>
#include <TimeTypes.h>

typedef bool (*PushReader)();

struct PushedEventArgs
{
    time_t pressedAt;
};

struct ReleasedEventArgs : PushedEventArgs
{
    time_t holdTime;
};

class Push
{
public:
    Push(PushReader reader, bool inverted, time_t debounce, bool bind = true);

    Push(byte pin, bool inverted, time_t debounce, bool bind = true);

    void update();

    bool current();

    bool pushed();

    bool released();

    time_t getHoldTime();

    void setReader(PushReader reader);

    void setReader(byte pin);

    Event<Push, PushedEventArgs&> push;

    Event<Push, ReleasedEventArgs&> release;

    time_t debounce;

private:
    PushReader m_Reader;
    ReleasedEventArgs m_ReleasedArgs;
    time_t m_LastDebounce;
    byte m_Status;
};

#endif