
#include "Colors.hh"

namespace pge {

auto toOlcPixel(const Color &color) -> olc::Pixel
{
  // Initially taken from here:
  // // https://github.com/matt-hayward/olcPGEX_AdditionalColours
  switch (color)
  {
    case Color::AMBER:
      return olc::Pixel(255, 191, 0);
    case Color::AMETHYST:
      return olc::Pixel(153, 102, 204);
    case Color::ANTIQUE_BRONZE:
      return olc::Pixel(102, 93, 30);
    case Color::APPLE_GREEN:
      return olc::Pixel(206, 255, 89);
    case Color::BIDOOF:
      return olc::Pixel(242, 225, 179);
    case Color::BLACK:
      return olc::BLACK;
    case Color::BLANK:
      return olc::BLANK;
    case Color::BLUE:
      return olc::BLUE;
    case Color::BRONZE:
      return olc::Pixel(205, 127, 50);
    case Color::BROWN:
      return olc::Pixel(199, 89, 27);
    case Color::COBALT_BLUE:
      return olc::Pixel(0, 160, 255);
    case Color::COPPER:
      return olc::Pixel(184, 115, 51);
    case Color::CORNFLOWER_BLUE:
      return olc::Pixel(100, 149, 237);
    case Color::CYAN:
      return olc::CYAN;
    case Color::GOLDEN:
      return olc::Pixel(255, 215, 0);
    case Color::GREEN:
      return olc::GREEN;
    case Color::GREY:
      return olc::GREY;
    case Color::INDIGO:
      return olc::Pixel(63, 0, 255);
    case Color::MAGENTA:
      return olc::MAGENTA;
    case Color::OFF_BLACK:
      return olc::Pixel(24, 24, 26);
    case Color::OFF_WHITE:
      return olc::Pixel(239, 248, 254);
    case Color::ORANGE:
      return olc::Pixel(255, 140, 0);
    case Color::PALE_COBALT:
      return olc::Pixel(194, 220, 255);
    case Color::PALE_CYAN:
      return olc::Pixel(192, 255, 240);
    case Color::PALE_GREEN:
      return olc::Pixel(202, 255, 192);
    case Color::PALE_ORANGE:
      return olc::Pixel(255, 229, 187);
    case Color::PALE_PINK:
      return olc::Pixel(255, 178, 216);
    case Color::PALE_PURPLE:
      return olc::Pixel(208, 186, 255);
    case Color::PALE_RED:
      return olc::Pixel(255, 179, 183);
    case Color::PALE_YELLOW:
      return olc::Pixel(255, 253, 181);
    case Color::PINK:
      return olc::Pixel(255, 121, 186);
    case Color::PLATINUM:
      return olc::Pixel(229, 228, 226);
    case Color::PURPLE:
      return olc::Pixel(208, 186, 255);
    case Color::RED:
      return olc::RED;
    case Color::SILVER:
      return olc::Pixel(192, 192, 192);
    case Color::ULTRAMARINE:
      return olc::Pixel(18, 10, 143);
    case Color::VERMILION:
      return olc::Pixel(227, 66, 52);
    case Color::WHITE:
      return olc::WHITE;
    case Color::YELLOW:
      return olc::YELLOW;

    case Color::DARK_BLUE:
      return olc::DARK_BLUE;
    case Color::DARK_COBALT_BLUE:
      return olc::Pixel(0, 97, 176);
    case Color::DARK_CYAN:
      return olc::DARK_CYAN;
    case Color::DARK_GREEN:
      return olc::DARK_GREEN;
    case Color::DARK_GREY:
      return olc::DARK_GREY;
    case Color::DARK_MAGENTA:
      return olc::DARK_MAGENTA;
    case Color::DARK_ORANGE:
      return olc::Pixel(255, 127, 0);
    case Color::DARK_RED:
      return olc::DARK_RED;
    case Color::DARK_YELLOW:
      return olc::DARK_YELLOW;

    case Color::VERY_DARK_BLUE:
      return olc::VERY_DARK_BLUE;
    case Color::VERY_DARK_COBALT_BLUE:
      return olc::Pixel(28, 56, 92);
    case Color::VERY_DARK_CYAN:
      return olc::VERY_DARK_CYAN;
    case Color::VERY_DARK_GREEN:
      return olc::VERY_DARK_GREEN;
    case Color::VERY_DARK_GREY:
      return olc::VERY_DARK_GREY;
    case Color::VERY_DARK_MAGENTA:
      return olc::VERY_DARK_MAGENTA;
    case Color::VERY_DARK_ORANGE:
      return olc::Pixel(255, 114, 0);
    case Color::VERY_DARK_RED:
      return olc::VERY_DARK_RED;
    case Color::VERY_DARK_YELLOW:
      return olc::VERY_DARK_YELLOW;

    case Color::TRANSPARENT_WHITE:
      return olc::Pixel{255, 255, 255, alpha::TRANSPARENT};
    case Color::TRANSPARENT_BLACK:
      return olc::Pixel{0, 0, 0, alpha::TRANSPARENT};

    default:
      throw std::invalid_argument("Unsupported color to convert to olc::Pixel");
  }
}

} // namespace pge