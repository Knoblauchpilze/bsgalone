
#pragma once

#include <string>

namespace pge {

template<class T>
class Vector2d
{
  public:
  Vector2d() = default;
  Vector2d(const T x, const T y);
  Vector2d(const Vector2d &rhs);

  auto operator=(const Vector2d &v) -> Vector2d & = default;

  auto norm() const -> T;
  auto norm2() const -> T;
  auto normalize() const -> Vector2d;
  auto normalized() -> Vector2d &;

  auto dot(const Vector2d &rhs) const -> T;
  auto cross(const Vector2d &rhs) const -> T;

  auto operator+(const Vector2d &rhs) const -> Vector2d;
  auto operator-(const Vector2d &rhs) const -> Vector2d;
  auto operator*(const T &rhs) const -> Vector2d;
  auto operator*(const Vector2d &rhs) const -> Vector2d;
  auto operator/(const T &rhs) const -> Vector2d;
  auto operator/(const Vector2d &rhs) const -> Vector2d;

  auto operator+=(const Vector2d &rhs) -> Vector2d &;
  auto operator-=(const Vector2d &rhs) -> Vector2d &;
  auto operator*=(const T &rhs) -> Vector2d &;
  auto operator/=(const T &rhs) -> Vector2d &;
  auto operator+() const -> Vector2d;
  auto operator-() const -> Vector2d;

  bool operator==(const Vector2d &rhs) const;
  bool operator!=(const Vector2d &rhs) const;

  auto str() const -> std::string;

  T x{0};
  T y{0};
};

using Vec2i = Vector2d<int>;
using Vec2f = Vector2d<float>;

} // namespace pge

#include "Vector2d.hxx"
