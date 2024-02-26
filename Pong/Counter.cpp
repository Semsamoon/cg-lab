#include "Counter.h"

using namespace Pong;

void Counter::Compose(Wall* pLeft, Wall* pRight)
{
    pLeft->CollidedEvent.AddRaw(this, &Counter::OnCollidedEvent, 1);
    pRight->CollidedEvent.AddRaw(this, &Counter::OnCollidedEvent, 2);
}

void Counter::OnCollidedEvent(bool _, int32 n)
{
    if (n == 1) points1++;
    else points2++;
    printf("%d - %d\n", points1, points2);
}
