
#pragma once

#include "Vector2d.hh"
#include <memory>

#include "olcEngine.hh"

namespace pge {

using Vectorf = olc::vf2d;

class Viewport
{
  public:
  /// @brief - Returns the center of this viewport.
  /// @return - the center of the viewport.
  virtual auto center() const noexcept -> Vectorf = 0;

  /// @brief - Returns the top left corner of this viewport.
  /// @return - the top left corner of the viewport.
  virtual auto topLeft() const noexcept -> Vectorf = 0;

  /// @brief - Returns the dimensions of the viewport.
  /// @return - the dimensions of the viewport.
  virtual auto dims() const noexcept -> Vectorf = 0;

  auto relativeCoords(const Vectorf &pos) const noexcept -> Vectorf;

  /// @brief - Returns the relative coordinates of the input position
  /// compared to this viewport. In this mode, the output coordinates
  /// range from `[0; 1]` when one axis is within the viewport.
  /// @param pos - the position to convert.
  /// @return - the relative coords for this position.
  virtual auto relativeCoords(const float x, const float y) const noexcept -> Vectorf = 0;

  auto absoluteCoords(const Vectorf &pos) const noexcept -> Vectorf;

  /// @brief - Returns the absolute coordinates of the input position
  /// compared to this viewport. In this mode, the input coordinates
  /// range from `[0; 1]` when one axis is within the viewport.
  /// @param pos - the position to convert.
  /// @return - the absolute coords for this position.
  virtual auto absoluteCoords(const float x, const float y) const noexcept -> Vectorf = 0;

  /// @brief - Moves this viewport to the new position. Depending
  /// on the semantic of the viewport it might have a different
  /// operation.
  /// @param center - the new position of the viewport.
  virtual void moveTo(const Vectorf &position) noexcept = 0;

  /// @brief - Translates this viewport by the specified amount.
  /// @param delta - the delta to apply to the position of this
  /// viewport.
  virtual void translate(const Vectorf &delta) noexcept = 0;

  void scale(const float factor) noexcept;

  /// @brief - Scales the dimensions of the viewport by the corresponding
  /// factor.
  /// @param sx - the factor to be applied on the `x` axis.
  /// @param sy - the factor to be applied on the `y` axis.
  virtual void scale(const float sx, const float sy) noexcept = 0;

  /// @brief - Whether an element having the specified coordinates and
  /// radius is visible in the viewport.
  /// Note: the radius is centered around the coordinates. The element
  /// extends from `[x - radius; x + radius]` and same for `y`.
  /// @param x - x coordinate of the point.
  /// @param y - y coordinate of the point.
  /// @param radius - the radius in both axis.
  /// @return - `true` if such an element is at least partially visible
  /// in the viewport.
  bool visible(const float x, const float y, const float radius = 1.0f) const noexcept;

  /// @brief - Whether an element having the specified coordinates and
  /// radius is visible in the viewport.
  /// Note: the size is centered around the coordinates. The element
  /// extends from `[x - sz.x; x + sz.x]` and same for `y`.
  /// @param p - the position of the element.
  /// @param sz - the size of the element.
  /// @return - `true` if such an element is at least partially visible
  /// in the viewport.
  bool visible(const Vectorf &p, const Vectorf &sz = Vectorf(1.0f, 1.0f)) const noexcept;

  /// @brief - Whether an element having the specified coordinates and
  /// size is visible in the viewport.
  /// Note: the size is centered around the coordinates. The element
  /// extends from `[x - sz.x; x + sz.x]` and same for `y`.
  /// @param x - the x cooridinate of the element.
  /// @param y - the y cooridinate of the element.
  /// @param sx - the radius of the element for the `x` axis.
  /// @param sy - the radius of the element for the `y` axis.
  /// @return - `true` if such an element is at least partially visible
  /// in the viewport.
  virtual bool visible(const float x,
                       const float y,
                       const float sx,
                       const float sy) const noexcept = 0;
};

using ViewportPtr = std::unique_ptr<Viewport>;
} // namespace pge
