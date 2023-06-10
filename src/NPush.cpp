#include "NPush.h"
#include <TimedInterval.h>
#include <Uptime.h>
#include <SketchBoundLibrary.h>

#include <Arduino.h>

#define s_r(b) bitRead(m_Status, b)
#define s_c(b) bitClear(m_Status, b)
#define s_s(b) bitSet(m_Status, b)
#define s_w(b, v) bitWrite(m_Status, b, v)
#define INVERTABLE(i_cond, expr) ((i_cond) ? !expr : expr)

enum StatusBits
{
    Current,
    Pushed,
    Released,
    Previous,
    r_Pushed,
    r_Released,
    Inverted,
    Virtual
};

Push::Push(PushReader reader, bool inverted, time_t debounce, bool bind)
    : push(Event<Push>()), release(Event<Push>()), debounce(debounce), m_Reader(nullptr), m_ReleasedArgs(ReleasedEventArgs()), m_LastDebounce(uptime()), m_UpdateCallable(VoidMemberVoid<Push>(this, &Push::update))
{
    s_w(Inverted, inverted);
    setReader(reader);
    if (bind)
        addSketchBinding(bind_loop, &m_UpdateCallable);
}

Push::Push(byte pin, bool inverted, time_t debounce, bool bind)
    : push(Event<Push>()), release(Event<Push>()), debounce(debounce), m_Reader(nullptr), m_ReleasedArgs(ReleasedEventArgs()), m_LastDebounce(uptime()), m_UpdateCallable(VoidMemberVoid<Push>(this, &Push::update))
{
    s_w(Inverted, inverted);
    setReader(pin);
    if (bind)
        addSketchBinding(bind_loop, &m_UpdateCallable);
}

void Push::update()
{
    if (s_r(Virtual) && !m_Reader)
        return;

    if (!intervalElapsed(m_LastDebounce, debounce))
        return;

    s_c(r_Pushed);
    s_c(r_Released);

    s_w
    (
        Current,
        INVERTABLE
        (
            s_r(Inverted),
            (s_r(Virtual) ?
                m_Reader()
                :
                digitalRead((byte)m_Reader)
            )
        )
    );

    if (!s_r(Current) && s_r(Previous))
    {
        s_s(Released);
        s_c(Previous);
        s_c(Pushed);
        m_ReleasedArgs.holdTime = getHoldTime();
        //release(&m_ReleasedArgs);
        return;
    }
    s_c(Released);

    if (s_r(Current) && !s_r(Previous))
    {
        s_s(Pushed);
        s_s(Previous);
        m_ReleasedArgs.pressedAt = uptime();
        //push(&m_ReleasedArgs);
        return;
    }
    s_c(Pushed);
}

bool Push::current()
{
    update();
    return s_r(Current);
}

bool Push::pushed()
{
    update();
    if (s_r(r_Pushed))
        return false;

    s_s(r_Pushed);
    return s_r(Pushed);
}

bool Push::released()
{
    update();
    if (s_r(r_Released))
        return false;
    s_s(r_Released);
    return s_r(Released);
}

time_t Push::getHoldTime()
{
    return uptime() - m_ReleasedArgs.pressedAt;
}

void Push::setReader(PushReader reader)
{
    s_s(Virtual);
    m_Reader = reader;
}

void Push::setReader(byte pin)
{
    s_c(Virtual);
    m_Reader = (PushReader)pin;
    pinMode((byte)pin, (s_r(Inverted) ? INPUT_PULLUP : INPUT));
}