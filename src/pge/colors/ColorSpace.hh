
#pragma once

#include "olcEngine.hh"

namespace pge {

auto colorGradient(const olc::Pixel &low, const olc::Pixel &high, float ratio, int alpha) noexcept
  -> olc::Pixel;

/// @brief - Convert the input color expressed in `RGB` color space to a color
/// in `HSL` space.
/// Note that some precision is lost when the output channels are converted to
/// integer values.
/// @param rgb - the input color to convert.
/// @return - the equivalent in `HSL` color space.
auto RGBToHSL(const olc::Pixel &rgb) noexcept -> olc::Pixel;

/// @brief - Convert the input color expressed in `HSL` color space to a color
/// in `RGB` space.
/// Note that some precision is lost when the output channels are converted to
/// integer values.
/// @param hsl - the input color to convert.
/// @return - the equivalent in `RGB` color space.
auto HSLToRGB(const olc::Pixel &hsl) noexcept -> olc::Pixel;

/// @brief - Modulate the luminance of the input color by the specified factor.
/// We convert the color into `HSL` space to change the `L` component and then
/// revert to `RGB` space.
/// @param in - the input color to modulate.
/// @param factor - a value indicating the change in lightness to perform. A value
/// larger than `1` will brighten the color and a value in the range `0` to `1`
/// will darken it. Any negative value will be clamped to `0`.
/// @return - the modulated color.
auto modulate(const olc::Pixel &in, float factor) noexcept -> olc::Pixel;

} // namespace pge
