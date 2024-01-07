
#pragma once

#include "CoordinateFrame.hh"
#include "Vector2d.hh"
#include <optional>
#include <string>

namespace pge {

/// @brief - Convenience description of settings to define the canvas used by
/// an app. This allows to be concise and easily add info if needed.
struct AppDesc
{
  /// The dimension of the window for this app in pixels.
  Vec2i dims{};

  /// The coordinate frame to use to convert dimensions in tiles to screen and
  /// pixels coordinates.
  CoordinateFramePtr frame{};

  /// A ratio defining how many 'logical' pixels will be mapped to 'screen'
  /// pixels.
  Vec2i enginePixToScreenPixRatio{1, 1};

  /// The name of the window.
  std::string name{"pge_app"};

  /// Whether or not the coordinate frame is fixed (meaning that panning and
  /// zooming is disabled) or not.
  bool fixedFrame{false};

  /// The maximum FPS that the app should try to maintain. If it is able to
  /// render faster than this, it will sleep to not exceed this value. The
  /// default is not provided, so it means the app will try to render as fast
  /// as possible.
  std::optional<int> maxFps{};
};

} // namespace pge
