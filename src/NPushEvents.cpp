#include "NPushEvents.h"

OnReleaseEventArgs::OnReleaseEventArgs()
    : holdTime(ZERO), pressedAt(ULONG_MAX)
{
}

OnReleaseEventArgs::OnReleaseEventArgs(uint16_t _holdTime, uint32_t _pressedAt)
    : holdTime(+holdTime), pressedAt(_pressedAt)
{
}

OnPushEventArgs::OnPushEventArgs()
    : pressedAt(ULONG_MAX)
{
}

OnPushEventArgs::OnPushEventArgs(uint32_t _pressedAt)
    : pressedAt(_pressedAt)
{
}