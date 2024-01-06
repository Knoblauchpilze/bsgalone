
#pragma once

#include "Viewport.hh"
#include "olcEngine.hh"
#include <maths_utils/Vector2.hh>

namespace pge {

/// @brief - Defines a viewport from its top left corner and size.
class TopLeftViewport : public Viewport
{
  public:
  /// @brief - Create a viewport with the specified size and top
  /// left corner.
  TopLeftViewport(const Vectorf &tl, const Vectorf &dims) noexcept;

  Vectorf center() const noexcept override;

  Vectorf topLeft() const noexcept override;

  Vectorf dims() const noexcept override;

  Vectorf relativeCoords(const float x, const float y) const noexcept override;

  Vectorf absoluteCoords(const float x, const float y) const noexcept override;

  /// @brief - Moves the reference corner of this viewport to
  /// the new position.
  /// @param topLeft - the new position of the reference corner
  /// for this viewport.
  void moveTo(const Vectorf &topLeft) noexcept override;

  /// @brief - Translates this viewport by the specified amount.
  /// @param delta - the delta to apply to the position of this
  /// viewport.
  void translate(const Vectorf &delta) noexcept override;

  /// @brief  - Keeps the reference corner constant but scales
  /// the dimensions by the specified factor.
  /// @param sx - the scale on the `x` axis.
  /// @param sy - the scale on the `y` axis.
  void scale(const float sx, const float sy) noexcept override;

  virtual bool visible(const float x,
                       const float y,
                       const float sx,
                       const float sy) const noexcept override;

  private:
  /// @brief - The top left corner of this viewport.
  Vectorf m_topLeft;

  /// @brief - The dimensions of the view along each axis.
  Vectorf m_dims;
};

} // namespace pge
