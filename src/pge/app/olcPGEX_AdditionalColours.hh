
#pragma once

// Initially taken from here:
// https://github.com/matt-hayward/olcPGEX_AdditionalColours/blob/develop/olcPGEX_AdditionalColours.h

#include "olcPixelGameEngine.h"

namespace olc {

static const Pixel OFF_BLACK(24, 24, 26);
static const Pixel OFF_WHITE(239, 248, 254);
static const Pixel ORANGE(255, 140, 0);
static const Pixel DARK_ORANGE(255, 127, 0);
static const Pixel VERY_DARK_ORANGE(255, 114, 0);
static const Pixel APPLE_GREEN(206, 255, 89);
static const Pixel DARK_APPLE_GREEN(174, 222, 0);
static const Pixel VERY_DARK_APPLE_GREEN(84, 107, 0);
static const Pixel COBALT_BLUE(0, 160, 255);
static const Pixel DARK_COBALT_BLUE(0, 97, 176);
static const Pixel VERY_DARK_COBALT_BLUE(28, 56, 92);
static const Pixel PURPLE(145, 61, 255);
static const Pixel DARK_PURPLE(96, 0, 222);
static const Pixel VERY_DARK_PURPLE(49, 0, 102);
static const Pixel PINK(255, 121, 186);
static const Pixel DARK_PINK(235, 0, 109);
static const Pixel VERY_DARK_PINK(107, 0, 43);
static const Pixel BROWN(199, 89, 27);
static const Pixel DARK_BROWN(117, 37, 0);
static const Pixel VERY_DARK_BROWN(51, 13, 0);
static const Pixel CORNFLOWER_BLUE(100, 149, 237);
static const Pixel DARK_CORNFLOWER_BLUE(80, 129, 217);
static const Pixel VERY_DARK_CORNFLOWER_BLUE(60, 109, 197);
static const Pixel BIDOOF(242, 225, 179);
static const Pixel DARK_BIDOOF(156, 117, 58);
static const Pixel VERY_DARK_BIDOOF(81, 68, 62);
static const Pixel PALE_RED(255, 179, 183);
static const Pixel PALE_ORANGE(255, 229, 187);
static const Pixel PALE_YELLOW(255, 253, 181);
static const Pixel PALE_GREEN(202, 255, 192);
static const Pixel PALE_CYAN(192, 255, 240);
static const Pixel PALE_COBALT(194, 220, 255);
static const Pixel PALE_PURPLE(208, 186, 255);
static const Pixel PALE_PINK(255, 178, 216);
static const Pixel PALE_DARK_RED(130, 66, 61);
static const Pixel PALE_DARK_ORANGE(133, 94, 61);
static const Pixel PALE_DARK_YELLOW(133, 117, 70);
static const Pixel PALE_DARK_GREEN(110, 133, 67);
static const Pixel PALE_DARK_CYAN(78, 133, 112);
static const Pixel PALE_DARK_COBALT(75, 114, 133);
static const Pixel PALE_DARK_PURPLE(83, 69, 133);
static const Pixel PALE_DARK_PINK(130, 66, 133);
static const Pixel AMETHYST(153, 102, 204);
static const Pixel AMBER(255, 191, 0);
static const Pixel GOLDEN(255, 215, 0);
static const Pixel SILVER(192, 192, 192);
static const Pixel PLATINUM(229, 228, 226);
static const Pixel ANTIQUE_BRONZE(102, 93, 30);
static const Pixel BRONZE(205, 127, 50);
static const Pixel COPPER(184, 115, 51);
static const Pixel VERMILION(227, 66, 52);
static const Pixel INDIGO(63, 0, 255);
static const Pixel ULTRAMARINE(18, 10, 143);

// Taken from https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
// Creates a new Pixel instance from hsv space (hue, saturation and value).
// Hue ranges from 0 to 360, saturation and value from 0 to 1.
inline auto fromHsv(float hue, float saturation, float value, const float alpha = 1) -> olc::Pixel
{
  hue        = std::clamp(hue, 0.0f, 360.0f);
  saturation = std::clamp(saturation, 0.0f, 1.0f);
  value      = std::clamp(value, 0.0f, 1.0f);

  const float chroma = value * saturation;
  const float x      = chroma * (1 - std::fabs(fmodf((hue / 60), 2) - 1));
  const float m      = value - chroma;

  float rawRed, rawGreen, rawBlue;

  if (saturation == 0)
  {
    rawRed = rawGreen = rawBlue = value;
  }
  else if (hue < 60)
  {
    rawRed   = chroma;
    rawGreen = x;
    rawBlue  = 0;
  }
  else if (60 <= hue && hue < 120)
  {
    rawRed   = x;
    rawGreen = chroma;
    rawBlue  = 0;
  }
  else if (120 <= hue && hue < 180)
  {
    rawRed   = 0;
    rawGreen = chroma;
    rawBlue  = x;
  }
  else if (180 <= hue && hue < 240)
  {
    rawRed   = 0;
    rawGreen = x;
    rawBlue  = chroma;
  }
  else if (240 <= hue && hue < 300)
  {
    rawRed   = x;
    rawGreen = 0;
    rawBlue  = chroma;
  }
  else
  {
    rawRed   = chroma;
    rawGreen = 0;
    rawBlue  = x;
  }

  return olc::PixelF(rawRed + m, rawGreen + m, rawBlue + m, alpha);
}
} // namespace olc
