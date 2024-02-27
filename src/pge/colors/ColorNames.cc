
#include "ColorNames.hh"

namespace pge::colors {

auto str(const Name name) -> std::string
{
  switch (name)
  {
    case Name::AMBER:
      return "amber";
    case Name::AMETHYST:
      return "amethyst";
    case Name::ANTIQUE_BRONZE:
      return "antique_bronze";
    case Name::APPLE_GREEN:
      return "apple_green";
    case Name::BIDOOF:
      return "bidoof";
    case Name::BLACK:
      return "black";
    case Name::BLANK:
      return "blank";
    case Name::BLUE:
      return "blue";
    case Name::BRONZE:
      return "bronze";
    case Name::BROWN:
      return "brown";
    case Name::COBALT_BLUE:
      return "cobalt_blue";
    case Name::COPPER:
      return "copper";
    case Name::CORNFLOWER_BLUE:
      return "cornflower_blue";
    case Name::CYAN:
      return "cyan";
    case Name::GOLDEN:
      return "golden";
    case Name::GREEN:
      return "green";
    case Name::GREY:
      return "grey";
    case Name::INDIGO:
      return "indigo";
    case Name::MAGENTA:
      return "magenta";
    case Name::OFF_BLACK:
      return "off_black";
    case Name::OFF_WHITE:
      return "off_white";
    case Name::ORANGE:
      return "orange";
    case Name::PALE_COBALT:
      return "pale_cobalt";
    case Name::PALE_CYAN:
      return "pale_cyan";
    case Name::PALE_GREEN:
      return "pale_green";
    case Name::PALE_ORANGE:
      return "pale_orange";
    case Name::PALE_PINK:
      return "pale_pink";
    case Name::PALE_PURPLE:
      return "pale_purple";
    case Name::PALE_RED:
      return "pale_red";
    case Name::PALE_YELLOW:
      return "pale_yellow";
    case Name::PINK:
      return "pink";
    case Name::PLATINUM:
      return "platinum";
    case Name::PURPLE:
      return "purple";
    case Name::RED:
      return "red";
    case Name::SILVER:
      return "silver";
    case Name::ULTRAMARINE:
      return "ultramarine";
    case Name::VERMILION:
      return "vermilion";
    case Name::WHITE:
      return "white";
    case Name::YELLOW:
      return "yellow";

    case Name::DARK_BLUE:
      return "dark_blue";
    case Name::DARK_COBALT_BLUE:
      return "dark_cobalt_blue";
    case Name::DARK_CYAN:
      return "dark_cyan";
    case Name::DARK_GREEN:
      return "dark_green";
    case Name::DARK_GREY:
      return "dark_grey";
    case Name::DARK_MAGENTA:
      return "dark_magenta";
    case Name::DARK_ORANGE:
      return "dark_orange";
    case Name::DARK_RED:
      return "dark_red";
    case Name::DARK_YELLOW:
      return "dark_yellow";

    case Name::VERY_DARK_BLUE:
      return "very_dark_blue";
    case Name::VERY_DARK_COBALT_BLUE:
      return "very_dark_cobalt_blue";
    case Name::VERY_DARK_CYAN:
      return "very_dark_cyan";
    case Name::VERY_DARK_GREEN:
      return "very_dark_green";
    case Name::VERY_DARK_GREY:
      return "very_dark_grey";
    case Name::VERY_DARK_MAGENTA:
      return "very_dark_magenta";
    case Name::VERY_DARK_ORANGE:
      return "very_dark_orange";
    case Name::VERY_DARK_RED:
      return "very_dark_red";
    case Name::VERY_DARK_YELLOW:
      return "very_dark_yellow";

    default:
      return "unknown";
  }
}

} // namespace pge::colors
