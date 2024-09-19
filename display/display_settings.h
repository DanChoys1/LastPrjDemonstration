#ifndef DISPLAY_SETTINGS_H
#define DISPLAY_SETTINGS_H

namespace DisplaySettings
{
/// Доля максимального смещение (зависящая от размера экрана) центра при истенном движении
constexpr double MaxCenterShiftCoeff = 2/3.0;

/// Чувствительность изменения пеленга визора
constexpr double VisorSensitiveBearing = 0.5;
/// Чувствительность изменения дальности визора
constexpr double VisorSensitiveDistance = 1;

/// Чувствительность изменения позиции курсора
constexpr double CursorSensitive = 1;
}

#endif // DISPLAY_SETTINGS_H
