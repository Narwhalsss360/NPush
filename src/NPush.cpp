#include "NPush.h"
#include <TimedInterval.h>
#include <Uptime.h>
#if __has_include(<SketchBoundLibrary.h>)
#include <SketchBoundLibrary.h>
#define NPush_Bindable
#endif

#define INVERTABLE(i_cond, expr) ((i_cond) ? !expr : expr)

Push::Push(PushReader reader, bool inverted, ntime_t debounce)
    : push(Event<Push, PushedEventArgs&>()), release(Event<Push, ReleasedEventArgs&>()), debounce(debounce), m_updateMethod(Method<Push, void>(this, &Push::update)), m_Reader(nullptr), m_ReleasedArgs(ReleasedEventArgs()), m_LastDebounce(uptime())
{
    m_Status.inverted = inverted;
    setReader(reader);
#ifdef NPush_Bindable
    addSketchBinding(bind_loop, &m_updateMethod);
#endif
}

Push::Push(byte pin, bool inverted, ntime_t debounce)
    : push(Event<Push, PushedEventArgs&>()), release(Event<Push, ReleasedEventArgs&>()), debounce(debounce), m_updateMethod(Method<Push, void>(this, &Push::update)), m_Reader(nullptr), m_ReleasedArgs(ReleasedEventArgs()), m_LastDebounce(uptime())
{
    m_Status.inverted = inverted;
    setReader(pin);
#ifdef NPush_Bindable
    addSketchBinding(bind_loop, &m_updateMethod);
#endif
}

void Push::update()
{
    if (m_Status.virtualReader && !m_Reader)
        return;

    if (!intervalElapsed(m_LastDebounce, debounce))
        return;

    m_Status.pushedStatus = false;
    m_Status.releasedStatus = false;


    m_Status.current = INVERTABLE
    (
        m_Status.inverted,
        (
            m_Status.virtualReader ?
                m_Reader()
                :
                digitalRead(*(byte*)&m_Reader)
        )
    );

    if (!m_Status.current && m_Status.previous)
    {
        m_Status.released = true;
        m_Status.previous = false;
        m_Status.pushed = false;
        m_ReleasedArgs.sender = this;
        m_ReleasedArgs.holdTime = getHoldTime();
        release(m_ReleasedArgs);
        return;
    }
    m_Status.released = false;

    if (m_Status.current && !m_Status.previous)
    {
        m_Status.pushed = true;
        m_Status.previous = true;
        m_ReleasedArgs.sender = this;
        m_ReleasedArgs.pressedAt = uptime();
        push(m_ReleasedArgs);
        return;
    }
    m_Status.pushed = false;
}

bool Push::current()
{
    update();
    return m_Status.current;
}

bool Push::pushed()
{
    update();
    if (m_Status.pushedStatus)
        return false;

    m_Status.pushedStatus = true;
    return true;
}

bool Push::released()
{
    update();
    if (m_Status.releasedStatus)
        return false;
    m_Status.releasedStatus = true;
    return true;
}

ntime_t Push::getHoldTime()
{
    return uptime() - m_ReleasedArgs.pressedAt;
}

void Push::setReader(PushReader reader)
{
    m_Status.virtualReader = true;
    m_Reader = reader;
}

void Push::setReader(byte pin)
{
    m_Status.virtualReader = false;
    m_Reader = (PushReader)pin;
    pinMode((byte)pin, (m_Status.inverted ? INPUT_PULLUP : INPUT));
}
