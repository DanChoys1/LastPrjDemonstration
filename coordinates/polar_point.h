#ifndef POLAR_POINT_H
#define POLAR_POINT_H

/// Точка в полярных координатах
struct PolarPoint
{
    /// Дальность
    double D = 0;
    /// Пеленг
    double B = 0;

    constexpr PolarPoint() = default;
    constexpr PolarPoint(double dist, double bear) :
        D(dist), B(bear)
    {}

    PolarPoint& operator *=(double scale)
    {
        D *= scale;
        return *this;
    }

    PolarPoint operator *(double scale)
    {
        PolarPoint p = *this;
        p *= scale;
        return p;
    }

    PolarPoint& operator /=(double scale)
    {
        D /= scale;
        return *this;
    }

    PolarPoint operator /(double scale)
    {
        PolarPoint p = *this;
        p /= scale;
        return p;
    }

    PolarPoint& operator +=(const PolarPoint& other)
    {
        D += other.D;
        B += other.B;
        return *this;
    }
    PolarPoint& operator -=(const PolarPoint& other)
    {
        D -= other.D;
        B -= other.B;
        return *this;
    }

    PolarPoint operator +(const PolarPoint& other)
    {
        return {this->D + other.D, this->B + other.B};
    }
    PolarPoint operator -(const PolarPoint& other)
    {
        return {this->D - other.D, this->B - other.B};
    }
};

#endif // POLAR_POINT_H
