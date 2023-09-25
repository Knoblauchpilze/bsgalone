
#pragma once

#include "CoordinateFrame.hh"
#include <maths_utils/Point2.hh>

namespace pge {

/// @brief - Convenience structure defining the resources
/// that can be displayed in any app. It contains pointers
/// to the world's data, to the frames allowing to change
/// from screen coordinates to world coordinates and the UI.
struct RenderState
{
  // The coordinate frame to convert tiles to pixels.
  CoordinateFrame &cf;

  /// @brief - Convenience method allowing to determine if an item is visible
  /// in the current viewport.
  /// @param p - the position to check in tiles.
  /// @param r - the radius of the item.
  /// @return - `true` if the object is at least partially visible.
  bool visible(const utils::Point2i &p, float r = 1.0f) const noexcept;

  /// @brief - Similar method to the above but for floating point position and
  /// a size instead of a radius which allows for non square objects.
  /// @param p - the position to check in tiles.
  /// @param sz - the size of the object.
  /// @return - `true` if the object is at least partially visible.
  bool visible(const olc::vf2d &p, const olc::vf2d sz = olc::vf2d(1.0f, 1.0f)) const noexcept;
};

} // namespace pge