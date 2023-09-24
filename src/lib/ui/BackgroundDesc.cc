
#include "BackgroundDesc.hh"

namespace pge::menu {

BackgroundDesc newColoredBackground(const olc::Pixel &c) noexcept
{
  olc::Pixel hsl = pge::RGBToHSL(c);

  BackgroundDesc bd;
  bd.color  = c;
  bd.hColor = pge::modulate(c, hsl.b > 127 ? 0.5f : 2.0f);

  bd.scale = true;

  return bd;
}

void updateFromColor(BackgroundDesc &bg, const olc::Pixel &c) noexcept
{
  // Generate a new background.
  BackgroundDesc nbg = newColoredBackground(c);

  // Restore the other props.
  nbg.scale = bg.scale;

  // Swap values.
  std::swap(bg, nbg);
}

} // namespace pge::menu
