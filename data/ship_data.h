#ifndef SHIP_DATA_H
#define SHIP_DATA_H

#include "coordinate_math.h"

namespace RlsData
{
    /// Данные по кораблю носителю
    struct SelfShip
    {
        qreal course = 0;
        qreal speed = 0;
        GeoPoint pos;
    };
}

#endif // SHIP_DATA_H
