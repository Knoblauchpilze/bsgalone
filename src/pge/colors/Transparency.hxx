
#pragma once

#include "Transparency.hh"

namespace pge {

constexpr auto makeTransparent(const Color &in, const uint8_t alpha) -> Color
{
  return Color{.name = in.name, .rgb = in.rgb, .alpha = alpha};
}

constexpr auto almostOpaque(const Color &in) -> Color
{
  return makeTransparent(in, alpha::ALMOST_OPAQUE);
}

constexpr auto semiOpaque(const Color &in) -> Color
{
  return makeTransparent(in, alpha::SEMI_OPAQUE);
}

constexpr auto almostTransparent(const Color &in) -> Color
{
  return makeTransparent(in, alpha::ALMOST_TRANSPARENT);
}

constexpr auto transparent(const Color &in) -> Color
{
  return makeTransparent(in, alpha::TRANSPARENT);
}

} // namespace pge
