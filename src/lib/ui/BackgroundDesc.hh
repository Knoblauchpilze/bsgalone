
#pragma once

#include "olcEngine.hh"

namespace pge::menu {

/// @brief - Convenience structure defining the background that can be applied to
/// a menu. We either define a color that is applied on all the menu or a sprite
/// that is repeated a certain number of times.
struct BackgroundDesc
{
  olc::Pixel color;
  olc::Pixel hColor;

  bool scale;
};

/// @brief - Initializes a background structure defining a color for the background.
/// @param c - the color to assign to the background.
/// @return - the created background object.
BackgroundDesc newColoredBackground(const olc::Pixel &c) noexcept;

/// @brief - Used to update the color and highlight color of the input background
/// description from the provided new one. Allows to keep the scale intact and not
/// expose the meaning of the update outside.
/// @param bg - the bg description to update.
/// @param c - the new base color to use for this bg desc. The highlight color will
/// be computed from it.
void updateFromColor(BackgroundDesc &bg, const olc::Pixel &c) noexcept;

} // namespace pge::menu
