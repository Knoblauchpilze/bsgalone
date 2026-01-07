
#pragma once

#include "Viewport.hh"

namespace pge {

/// @brief - Defines a viewport from its top left corner and size.
class TopLeftViewport : public Viewport
{
  public:
  /// @brief - Create a viewport with the specified size and top
  /// left corner.
  TopLeftViewport(const Vec2f &tl, const Vec2f &dims) noexcept;
  ~TopLeftViewport() override = default;

  auto center() const noexcept -> Vec2f override;
  auto topLeft() const noexcept -> Vec2f override;
  auto dims() const noexcept -> Vec2f override;
  auto relativeCoords(const float x, const float y) const noexcept -> Vec2f override;
  auto absoluteCoords(const float x, const float y) const noexcept -> Vec2f override;

  /// @brief - Moves the reference corner of this viewport to
  /// the new position.
  /// @param topLeft - the new position of the reference corner
  /// for this viewport.
  void moveTo(const Vec2f &topLeft) noexcept override;

  /// @brief - Translates this viewport by the specified amount.
  /// @param delta - the delta to apply to the position of this
  /// viewport.
  void translate(const Vec2f &delta) noexcept override;

  /// @brief  - Keeps the reference corner constant but scales
  /// the dimensions by the specified factor.
  /// @param sx - the scale on the `x` axis.
  /// @param sy - the scale on the `y` axis.
  void scale(const float sx, const float sy) noexcept override;

  bool visible(const float x, const float y, const float sx, const float sy) const noexcept override;

  private:
  /// @brief - The top left corner of this viewport.
  Vec2f m_topLeft;

  /// @brief - The dimensions of the view along each axis.
  Vec2f m_dims;
};

} // namespace pge
