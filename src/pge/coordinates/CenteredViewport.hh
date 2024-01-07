
#pragma once

#include "Viewport.hh"
#include "olcEngine.hh"
#include <maths_utils/Vector2.hh>

namespace pge {

/// @brief - Defines a viewport from its center and size.
class CenteredViewport : public Viewport
{
  public:
  /// @brief - Create a viewport with the specified size and center.
  /// The dimensions are spread equally on both sides of the center
  /// so the effective extent is from `center.x - dims.x / 2` to
  /// `center.x + dims.x / 2` and same along the `y` axis.
  /// @param center - the center of the viewport.
  /// @param dims - the dimensions of the viewport.
  CenteredViewport(const Vectorf &center, const Vectorf &dims) noexcept;

  auto center() const noexcept -> Vectorf override;
  auto topLeft() const noexcept -> Vectorf override;
  auto dims() const noexcept -> Vectorf override;
  auto relativeCoords(const float x, const float y) const noexcept -> Vectorf override;
  auto absoluteCoords(const float x, const float y) const noexcept -> Vectorf override;

  /// @brief - Moves the center of this viewport to the new position.
  /// @param topLeft - the new position of the center for this viewport.
  void moveTo(const Vectorf &center) noexcept override;

  /// @brief - Translates this viewport by the specified amount.
  /// @param delta - the delta to apply to the position of this
  /// viewport.
  void translate(const Vectorf &delta) noexcept override;

  /// @brief  - Keeps the center constant but scales the dimensions
  /// by the specified factor.
  /// @param sx - the scale on the `x` axis.
  /// @param sy - the scale on the `y` axis.
  void scale(const float sx, const float sy) noexcept override;

  bool visible(const float x, const float y, const float sx, const float sy) const noexcept override;

  private:
  /// @brief - The center of this viewport.
  Vectorf m_center;

  /// @brief - The dimensions of the viewport along each axis.
  Vectorf m_dims;
};

} // namespace pge
