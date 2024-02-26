#pragma once

#include "Wall.h"

#include "../Lib/Types.h"

namespace Pong
{
    class Counter
    {
    public:
        void Compose(Wall* pLeft, Wall* pRight);

    private:
        void OnCollidedEvent(bool _, int32 n);

        int32 points1 = 0;
        int32 points2 = 0;
    };
}
