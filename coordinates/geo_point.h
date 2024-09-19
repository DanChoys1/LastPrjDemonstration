#ifndef GEO_POINT_H
#define GEO_POINT_H

/// Точка в географических координатах
struct GeoPoint
{
    /// Широта
    double lat = 0;
    /// Долгота
    double lon = 0;
    /// Широта от центра карты
    double centerLat = 0;

    GeoPoint() = default;
    GeoPoint(double lat, double lon, double centerLat = 0) :
        lat(lat), lon(lon), centerLat(centerLat)
    {}
    GeoPoint(const GeoPoint& geoPoint, double centerLat = 0) :
        lat(geoPoint.lat), lon(geoPoint.lon), centerLat(centerLat)
    {}

    GeoPoint& operator += (const GeoPoint& other)
    {
        lat += other.lat;
        lon += other.lon;
        return *this;
    }

    GeoPoint operator + (const GeoPoint& other) const
    {
        GeoPoint newGeoPoint = *this;
        newGeoPoint += other;
        return newGeoPoint;
    }

    GeoPoint operator - (const GeoPoint& other) const
    {
        return { lat - other.lat, lon - other.lon };
    }
};

#endif // GEO_POINT_H
