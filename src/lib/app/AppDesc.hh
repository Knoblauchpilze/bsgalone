
#pragma once

#include "CoordinateFrame.hh"
#include "olcEngine.hh"
#include <optional>
#include <string>

namespace pge {

/// @brief - Convenience description of settings to define the canvas used by
/// an app. This allows to be concise and easily add info if needed.
struct AppDesc
{
  // The dimension of the window for this app in pixels.
  olc::vi2d dims;

  // A ratio defining how many 'logical' pixels will be
  // mapped to 'screen' pixels.
  olc::vi2d pixRatio;

  // The name of the window.
  std::string name;

  // The coordinate frame to use to convert dimensions in
  // tiles to screen and pixels coordinates.
  CoordinateFramePtr frame;

  // Whether or not the coordinate frame is fixed (meaning
  // that panning and zooming is disabled) or not.
  bool fixedFrame;

  // The maximum FPS that the app should try to maintain. If
  // it is able to render faster than this, it will sleep to
  // not exceed this value. The default is not provided, so
  // it means the app will try to render as fast as possible.
  std::optional<int> maxFps{};
};

/// @brief - Creates a default app description.
/// @param dims - the dimensions of the application's main canvas in pixels.
/// @param cf - the coordinate frame to associate to the application.
/// @param name - the name of the application.
/// @return - the default description of an app.
AppDesc newDesc(const olc::vi2d &dims,
                CoordinateFramePtr cf,
                const std::string &name = "pge_app") noexcept;

} // namespace pge

#include "AppDesc.hxx"
