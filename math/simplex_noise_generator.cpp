#include "simplex_noise_generator.h"

#include <cmath>

SimplexNoiseGenerator::SimplexNoiseGenerator(int octaves, double persistence, double lacunarity, long seed)
{
    setSettings(octaves, persistence, lacunarity);
    setSeed(seed);
}

void SimplexNoiseGenerator::setSettings(int octaves, double persistence, double lacunarity)
{
    _octaves = octaves;
    _persistence = persistence;
    _lacunarity = lacunarity;
}

void SimplexNoiseGenerator::setSeed(long seed)
{
    if (_seed == seed) return;

    _seed = seed;

    int source[256];
    for (int i = 0; i < 256; ++i)
    {
        source[i] = i;
    }

    seed = seed * 6364136223846793005L + 1442695040888963407L;
    seed = seed * 6364136223846793005L + 1442695040888963407L;
    seed = seed * 6364136223846793005L + 1442695040888963407L;

    for (int i = 255; i >= 0; --i)
    {
        seed = seed * 6364136223846793005L + 1442695040888963407L;
        int r = (int)((seed + 31) % (i + 1));
        if (r < 0)
        {
            r += (i + 1);
        }
        _perm[i] = source[r];
        _perm[i + 256] = source[r];
        source[r] = source[i];
    }
}

double SimplexNoiseGenerator::noise(double x, double y) const
{
    double noiseValue = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;
    double maxAmplitude = 0.0;

    for (int i = 0; i < _octaves; ++i)
    {
        noiseValue += eval(x * frequency, y * frequency) * amplitude;
        frequency *= _lacunarity;
        maxAmplitude += amplitude;
        amplitude *= _persistence;
    }

    return noiseValue / maxAmplitude;
}

double SimplexNoiseGenerator::eval(double x, double y) const
{
    static const double F2 = 0.5 * (sqrt(3.0) - 1.0);
    static const double G2 = (3.0 - sqrt(3.0)) / 6.0;

    double s = (x + y) * F2;
    int i = std::floor(x + s);
    int j = std::floor(y + s);

    double t = (i + j) * G2;
    double X0 = i - t;
    double Y0 = j - t;
    double x0 = x - X0;
    double y0 = y - Y0;

    int ii = i & 255;
    int jj = j & 255;

    double t0 = 0.5 - x0 * x0 - y0 * y0;
    double n0 = t0 < 0 ? 0 : std::pow(t0, 4) * grad(_perm[ii + _perm[jj]], x0, y0);

    int i1 = x0 > y0 ? 1 : 0;
    int j1 = !i1;

    double x1 = x0 - i1 + G2;
    double y1 = y0 - j1 + G2;
    double t1 = 0.5 - x1 * x1 - y1 * y1;
    double n1 = t1 < 0 ? 0 : std::pow(t1, 4) * grad(_perm[ii + i1 + _perm[jj + j1]], x1, y1);

    double x2 = x0 - 1.0 + 2.0 * G2;
    double y2 = y0 - 1.0 + 2.0 * G2;
    double t2 = 0.5 - x2 * x2 - y2 * y2;
    double n2 = t2 < 0 ? 0 : std::pow(t2, 4) * grad(_perm[ii + 1 + _perm[jj + 1]], x2, y2);

    return 70.0 * (n0 + n1 + n2);
}

double SimplexNoiseGenerator::grad(int hash, double x, double y) const
{
    int h = hash & 7;
    double u = h < 4 ? x : y;
    double v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
}
