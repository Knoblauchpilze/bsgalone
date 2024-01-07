
#pragma once

#include "Vector2d.hh"
#include <cmath>

namespace pge {

template<class T>
inline Vector2d<T>::Vector2d(const T x, const T y)
  : x(x)
  , y(y)
{}

template<class T>
inline Vector2d<T>::Vector2d(const Vector2d &rhs)
  : x(rhs.x)
  , y(rhs.y)
{}

template<class T>
inline auto Vector2d<T>::norm() const -> T
{
  return static_cast<T>(std::sqrt(norm2()));
}

template<class T>
inline auto Vector2d<T>::norm2() const -> T
{
  return x * x + y * y;
}

template<class T>
inline auto Vector2d<T>::normalize() const -> Vector2d
{
  const auto r = static_cast<T>(1) / norm2();
  return Vector2d(x * r, y * r);
}

template<class T>
inline auto Vector2d<T>::normalized() -> Vector2d &
{
  const auto r = static_cast<T>(1) / norm2();
  x *= r;
  y *= r;
  return *this;
}

template<class T>
inline auto Vector2d<T>::dot(const Vector2d &rhs) const -> T
{
  return x * rhs.x + y * rhs.y;
}

template<class T>
inline auto Vector2d<T>::cross(const Vector2d &rhs) const -> T
{
  return x * rhs.y - y * rhs.x;
}

template<class T>
inline auto Vector2d<T>::operator+(const Vector2d &rhs) const -> Vector2d
{
  return Vector2d(x + rhs.x, y + rhs.y);
}

template<class T>
inline auto Vector2d<T>::operator-(const Vector2d &rhs) const -> Vector2d
{
  return Vector2d(x - rhs.x, y - rhs.y);
}

template<class T>
inline auto Vector2d<T>::operator*(const T &rhs) const -> Vector2d
{
  return Vector2d(x * rhs, y * rhs);
}

template<class T>
inline auto Vector2d<T>::operator*(const Vector2d &rhs) const -> Vector2d
{
  return Vector2d(x * rhs.x, y * rhs.y);
}

template<class T>
inline auto Vector2d<T>::operator/(const T &rhs) const -> Vector2d
{
  return Vector2d(x / rhs, y / rhs);
}

template<class T>
inline auto Vector2d<T>::operator/(const Vector2d &rhs) const -> Vector2d
{
  return Vector2d(x / rhs.x, y / rhs.y);
}

template<class T>
inline auto Vector2d<T>::operator+=(const Vector2d &rhs) -> Vector2d &
{
  x += rhs.x;
  y += rhs.y;
  return *this;
}

template<class T>
inline auto Vector2d<T>::operator-=(const Vector2d &rhs) -> Vector2d &
{
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

template<class T>
inline auto Vector2d<T>::operator*=(const T &rhs) -> Vector2d &
{
  x *= rhs;
  y *= rhs;
  return *this;
}

template<class T>
inline auto Vector2d<T>::operator/=(const T &rhs) -> Vector2d &
{
  x /= rhs;
  y /= rhs;
  return *this;
}

template<class T>
inline auto Vector2d<T>::operator+() const -> Vector2d
{
  return Vector2d(+x, +y);
}

template<class T>
inline auto Vector2d<T>::operator-() const -> Vector2d
{
  return Vector2d(-x, -y);
}

template<class T>
bool Vector2d<T>::operator==(const Vector2d &rhs) const
{
  return x == rhs.x && y == rhs.y;
}

template<class T>
bool Vector2d<T>::operator!=(const Vector2d &rhs) const
{
  return x != rhs.x || y != rhs.y;
}

template<class T>
inline auto Vector2d<T>::str() const -> std::string
{
  return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}

template<class T>
inline Vector2d<T> operator*(const float lhs, const Vector2d<T> &rhs)
{
  return Vector2d(lhs * rhs.x, lhs * rhs.y);
}

template<class T>
inline Vector2d<T> operator*(const double lhs, const Vector2d<T> &rhs)
{
  return Vector2d(lhs * rhs.x, lhs * rhs.y);
}

template<class T>
inline Vector2d<T> operator*(const int lhs, const Vector2d<T> &rhs)
{
  return Vector2d(lhs * rhs.x, lhs * rhs.y);
}

template<class T>
inline Vector2d<T> operator/(const float lhs, const Vector2d<T> &rhs)
{
  return Vector2d(lhs / rhs.x, lhs / rhs.y);
}

template<class T>
inline Vector2d<T> operator/(const double lhs, const Vector2d<T> &rhs)
{
  return Vector2d(lhs / rhs.x, lhs / rhs.y);
}

template<class T>
inline Vector2d<T> operator/(const int lhs, const Vector2d<T> &rhs)
{
  return Vector2d(lhs / rhs.x, lhs / rhs.y);
}

} // namespace pge
