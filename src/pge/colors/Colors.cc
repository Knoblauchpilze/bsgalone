
#include "Colors.hh"

namespace pge::colors {

auto toOlcPixel(const Name &name) -> olc::Pixel
{
  // Initially taken from here:
  // // https://github.com/matt-hayward/olcPGEX_AdditionalColours
  switch (name)
  {
    case Name::AMBER:
      return olc::Pixel(255, 191, 0);
    case Name::AMETHYST:
      return olc::Pixel(153, 102, 204);
    case Name::ANTIQUE_BRONZE:
      return olc::Pixel(102, 93, 30);
    case Name::APPLE_GREEN:
      return olc::Pixel(206, 255, 89);
    case Name::BIDOOF:
      return olc::Pixel(242, 225, 179);
    case Name::BLACK:
      return olc::BLACK;
    case Name::BLANK:
      return olc::BLANK;
    case Name::BLUE:
      return olc::BLUE;
    case Name::BRONZE:
      return olc::Pixel(205, 127, 50);
    case Name::BROWN:
      return olc::Pixel(199, 89, 27);
    case Name::COBALT_BLUE:
      return olc::Pixel(0, 160, 255);
    case Name::COPPER:
      return olc::Pixel(184, 115, 51);
    case Name::CORNFLOWER_BLUE:
      return olc::Pixel(100, 149, 237);
    case Name::CYAN:
      return olc::CYAN;
    case Name::GOLDEN:
      return olc::Pixel(255, 215, 0);
    case Name::GREEN:
      return olc::GREEN;
    case Name::GREY:
      return olc::GREY;
    case Name::INDIGO:
      return olc::Pixel(63, 0, 255);
    case Name::MAGENTA:
      return olc::MAGENTA;
    case Name::OFF_BLACK:
      return olc::Pixel(24, 24, 26);
    case Name::OFF_WHITE:
      return olc::Pixel(239, 248, 254);
    case Name::ORANGE:
      return olc::Pixel(255, 140, 0);
    case Name::PALE_COBALT:
      return olc::Pixel(194, 220, 255);
    case Name::PALE_CYAN:
      return olc::Pixel(192, 255, 240);
    case Name::PALE_GREEN:
      return olc::Pixel(202, 255, 192);
    case Name::PALE_ORANGE:
      return olc::Pixel(255, 229, 187);
    case Name::PALE_PINK:
      return olc::Pixel(255, 178, 216);
    case Name::PALE_PURPLE:
      return olc::Pixel(208, 186, 255);
    case Name::PALE_RED:
      return olc::Pixel(255, 179, 183);
    case Name::PALE_YELLOW:
      return olc::Pixel(255, 253, 181);
    case Name::PINK:
      return olc::Pixel(255, 121, 186);
    case Name::PLATINUM:
      return olc::Pixel(229, 228, 226);
    case Name::PURPLE:
      return olc::Pixel(208, 186, 255);
    case Name::RED:
      return olc::RED;
    case Name::SILVER:
      return olc::Pixel(192, 192, 192);
    case Name::ULTRAMARINE:
      return olc::Pixel(18, 10, 143);
    case Name::VERMILION:
      return olc::Pixel(227, 66, 52);
    case Name::WHITE:
      return olc::WHITE;
    case Name::YELLOW:
      return olc::YELLOW;

    case Name::DARK_BLUE:
      return olc::DARK_BLUE;
    case Name::DARK_COBALT_BLUE:
      return olc::Pixel(0, 97, 176);
    case Name::DARK_CYAN:
      return olc::DARK_CYAN;
    case Name::DARK_GREEN:
      return olc::DARK_GREEN;
    case Name::DARK_GREY:
      return olc::DARK_GREY;
    case Name::DARK_MAGENTA:
      return olc::DARK_MAGENTA;
    case Name::DARK_ORANGE:
      return olc::Pixel(255, 127, 0);
    case Name::DARK_RED:
      return olc::DARK_RED;
    case Name::DARK_YELLOW:
      return olc::DARK_YELLOW;

    case Name::VERY_DARK_BLUE:
      return olc::VERY_DARK_BLUE;
    case Name::VERY_DARK_COBALT_BLUE:
      return olc::Pixel(28, 56, 92);
    case Name::VERY_DARK_CYAN:
      return olc::VERY_DARK_CYAN;
    case Name::VERY_DARK_GREEN:
      return olc::VERY_DARK_GREEN;
    case Name::VERY_DARK_GREY:
      return olc::VERY_DARK_GREY;
    case Name::VERY_DARK_MAGENTA:
      return olc::VERY_DARK_MAGENTA;
    case Name::VERY_DARK_ORANGE:
      return olc::Pixel(255, 114, 0);
    case Name::VERY_DARK_RED:
      return olc::VERY_DARK_RED;
    case Name::VERY_DARK_YELLOW:
      return olc::VERY_DARK_YELLOW;

    case Name::TRANSPARENT_WHITE:
      return olc::Pixel{255, 255, 255, alpha::TRANSPARENT};
    case Name::TRANSPARENT_BLACK:
      return olc::Pixel{0, 0, 0, alpha::TRANSPARENT};

    default:
      throw std::invalid_argument("Unsupported color to convert to olc::Pixel");
  }
}

} // namespace pge::colors
