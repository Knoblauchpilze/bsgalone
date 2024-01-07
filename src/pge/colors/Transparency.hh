
#pragma once

#include "olcEngine.hh"

namespace pge {
namespace alpha {
/// @brief  @brief - Alpha values for various transparency levels.
constexpr uint8_t OPAQUE             = 255;
constexpr uint8_t ALMOST_OPAQUE      = 192;
constexpr uint8_t SEMI_OPAQUE        = 128;
constexpr uint8_t ALMOST_TRANSPARENT = 64;
constexpr uint8_t TRANSPARENT        = 0;
} // namespace alpha

auto opacityFromPercentage(const float perc) -> uint8_t;

/// @brief - Return a modified version of the input color with the
/// desired alpha value.
/// @param in - the input color.
/// @param alpha - the alpha to apply to the color.
/// @return - the input color with the desired transparency.
auto makeTransparent(const olc::Pixel &in, const uint8_t alpha = alpha::TRANSPARENT) -> olc::Pixel;

/// @brief - Return a modified version of the input color with the
/// alpha value computed from the percentage. In case the percentage is
/// not in the range [0; 1] it will be clamped.
/// @param in - the input color
/// @param perc - the percentage (in the range [0; 1]) for transparency.
/// @return - the modified transparent color.
auto makeTransparentFromPercentage(const olc::Pixel &in, const float perc) -> olc::Pixel;

auto almostOpaque(const olc::Pixel &in) -> olc::Pixel;
auto semiOpaque(const olc::Pixel &in) -> olc::Pixel;
auto almostTransparent(const olc::Pixel &in) -> olc::Pixel;
auto transparent(const olc::Pixel &in) -> olc::Pixel;

} // namespace pge