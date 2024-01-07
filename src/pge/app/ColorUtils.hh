
#pragma once

#include "olcEngine.hh"

namespace pge {

olc::Pixel colorGradient(const olc::Pixel &low,
                         const olc::Pixel &high,
                         float ratio,
                         int alpha) noexcept;

/// @brief - Convert the input color expressed in `RGB` color space to a color
/// in `HSL` space.
/// Note that some precision is lost when the output channels are converted to
/// integer values.
/// @param rgb - the input color to convert.
/// @return - the equivalent in `HSL` color space.
olc::Pixel RGBToHSL(const olc::Pixel &rgb) noexcept;

/// @brief - Convert the input color expressed in `HSL` color space to a color
/// in `RGB` space.
/// Note that some precision is lost when the output channels are converted to
/// integer values.
/// @param hsl - the input color to convert.
/// @return - the equivalent in `RGB` color space.
olc::Pixel HSLToRGB(const olc::Pixel &hsl) noexcept;

/// @brief - Modulate the lightness of the input color by the specified factor.
/// We convert the color into `HSL` space to change the `L` component and then
/// revert to `RGB` space.
/// @param in - the input color to modulate.
/// @param factor - a value indicating the change in lightness to perform. A value
/// *larger than `1` will brighten the *color and a value in the range `0` *to `1`
/// will darken it. Any negative *value will be clamped to `0`.
/// @return -the modulated color.
olc::Pixel modulate(const olc::Pixel &in, float factor) noexcept;

namespace alpha {

/// @brief - Alpha value for an opaque color.
constexpr uint8_t Opaque = 255;

/// @brief - Alpha value for an almost opaque color.
constexpr uint8_t AlmostOpaque = 192;

/// @brief - Alpha value for a semi opaque color.
constexpr uint8_t SemiOpaque = 128;

/// @brief - Alpha value for an almost transparent color.
constexpr uint8_t AlmostTransparent = 64;

/// @brief - Alpha value for a transparent color.
constexpr uint8_t Transparent = 0;

} // namespace alpha

/// @brief - Return a modified version of the input color with the
/// desired alpha value.
/// @param in - the input color.
/// @param alpha - the alpha to apply to the color.
/// @return - the input color with the desired transparency.
auto transparent(const olc::Pixel &in, const uint8_t alpha = alpha::Transparent) -> olc::Pixel;

auto opacityFromPercentage(const float perc) -> uint8_t;

auto opacifyFromPercentage(const olc::Pixel &in, const float perc) -> olc::Pixel;

/// @brief - Provides a human readable display of a color.
/// @param p - the pixel to convert to a string.
/// @return - a human readable representing the color.
auto str(const olc::Pixel &p) noexcept -> std::string;

} // namespace pge

#include "ColorUtils.hxx"
