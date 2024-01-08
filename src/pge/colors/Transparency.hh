
#pragma once

#include "Color.hh"

namespace pge {

auto opacityFromPercentage(const float perc) -> uint8_t;

/// @brief - Return a modified version of the input color with the
/// desired alpha value.
/// @param in - the input color.
/// @param alpha - the alpha to apply to the color.
/// @return - the input color with the desired transparency.
constexpr auto makeTransparent(const Color &in, const uint8_t alpha) -> Color;

/// @brief - Return a modified version of the input color with the
/// alpha value computed from the percentage. In case the percentage is
/// not in the range [0; 1] it will be clamped.
/// @param in - the input color
/// @param perc - the percentage (in the range [0; 1]) for transparency.
/// @return - the modified transparent color.
auto makeTransparentFromPercentage(const Color &in, const float perc) -> Color;

constexpr auto almostOpaque(const Color &in) -> Color;
constexpr auto semiOpaque(const Color &in) -> Color;
constexpr auto almostTransparent(const Color &in) -> Color;
constexpr auto transparent(const Color &in) -> Color;

} // namespace pge

#include "Transparency.hxx"
