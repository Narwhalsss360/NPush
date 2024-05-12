#ifndef NPush_h
#define NPush_h

#include <Arduino.h>
#include <NEvents.h>
#include <TimeTypes.h>

typedef bool (*PushReader)();

struct PushedEventArgs
{
    ntime_t pressedAt;
};

struct ReleasedEventArgs : PushedEventArgs
{
    ntime_t holdTime;
};

class Push
{
public:
    Push(PushReader reader, bool inverted, ntime_t debounce, bool bind = true);

    Push(byte pin, bool inverted, ntime_t debounce, bool bind = true);

    /// @brief Read reader state, interpret button state.
    void update();

    /// @brief Simple reader state read.
    /// @return `bool` reader state
    bool current();

    /// @brief Whether the button was pushed since last check.
    /// @return `bool` was pushed
    bool pushed();

    /// @brief Whether the button was released since last check.
    /// @return `bool` was released
    bool released();

    /// @brief Gets the time the button was held for. (Call after `released()` returns `true`).
    /// @return `time_t` time held
    ntime_t getHoldTime();

    /// @brief Change the state reader. (`PushReader` overload).
    /// @param reader reader
    void setReader(PushReader reader);

    /// @brief Change the state reader. (`byte`|`pin` overload).
    /// @param pin pin
    void setReader(byte pin);

    /// @brief Event invoked when button pushed. (caller: `update()`)
    Event<Push, PushedEventArgs&> push;

    /// @brief Event invoked when button released. (caller: `update()`)
    Event<Push, ReleasedEventArgs&> release;

    /// @brief Debounce time for reader (ignore time).
    ntime_t debounce;

private:
    PushReader m_Reader;
    ReleasedEventArgs m_ReleasedArgs;
    ntime_t m_LastDebounce;
    byte m_Status;
};

#endif