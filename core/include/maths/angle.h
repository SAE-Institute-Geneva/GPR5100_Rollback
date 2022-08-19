#pragma once

#include <cmath>

namespace core
{

inline constexpr static float PI = static_cast<float>(M_PI);
class Degree;

class Radian
{
public:
    constexpr Radian() = default;
    constexpr Radian(float value) : value_(value){}
    constexpr Radian(const Degree& angle);
    [[nodiscard]] constexpr float value() const { return value_; }
private:
    float value_ = 0.0f;
};


class Degree
{
public:
    constexpr Degree() = default;;
    constexpr Degree(float value) : value_(value){}
    constexpr Degree(const Radian& radian) : value_(radian.value()/PI*180.0f){}
    [[nodiscard]] constexpr float value() const { return value_; }
    constexpr Degree operator+(Degree angle) const { return { value_ + angle.value() }; }
    constexpr Degree& operator+=(Degree angle)
    {
        value_ += angle.value();
        return *this;
    }
    constexpr Degree operator-(Degree angle) const { return { value_ - angle.value() }; }
    constexpr Degree operator*(float value) const { return { value_ * value }; }
    constexpr Degree operator/(float value) const { return { value_ / value }; }
    constexpr Degree operator-() const { return { -value_ }; }
private:
    float value_ = 0.0f;
};

constexpr Degree operator*(float value, Degree angle) { return angle.value() * value; };


constexpr Radian::Radian(const Degree& angle)
{
    value_ = angle.value() / 180.0f * PI;
}

inline float Sin(Radian angle)
{
    return std::sin(angle.value());
}
inline float Cos(Radian angle)
{
    return std::cos(angle.value());
}
inline float Tan(Radian angle)
{
    return std::tan(angle.value());
}
inline Radian Asin(float ratio)
{
    return Radian(std::asin(ratio));
}
inline Radian Acos(float ratio)
{
    return Radian(std::acos(ratio));
}
inline Radian Atan(float ratio)
{
    return Radian(std::atan(ratio));
}

inline Radian Atan2(float a, float b)
{
    return Radian(std::atan2(a,b));
}
}