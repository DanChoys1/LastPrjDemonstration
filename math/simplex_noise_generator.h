#ifndef SIMPLEX_NOISE_GENERATOR_H
#define SIMPLEX_NOISE_GENERATOR_H

/// Генератор симплексного шума
class SimplexNoiseGenerator
{
public:
    SimplexNoiseGenerator(int octaves = 0, double persistence = 0, double lacunarity = 0, long seed = 0);
    ~SimplexNoiseGenerator() = default;

    /// Настройки генератора
    void setSettings(int octaves, double persistence, double lacunarity);
    /// Сид
    void setSeed(long seed);
    /// Генерация шума в координатах
    double noise(double x, double y) const;

private:
    double eval(double x, double y) const;
    double grad(int hash, double x, double y) const;

private:
    unsigned char _perm[512];
    long _seed = 0;

    double _octaves = 0;
    double _persistence = 0;
    double _lacunarity = 0;

};

#endif // SIMPLEX_NOISE_GENERATOR_H
