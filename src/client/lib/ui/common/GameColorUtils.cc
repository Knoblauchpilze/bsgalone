
#include "GameColorUtils.hh"

namespace pge {
constexpr auto TYLIUM_NAME = "tylium";
constexpr auto TITANE_NAME = "titane";

auto colorFromResourceName(const std::string &name) -> Color
{
  if (TYLIUM_NAME == name)
  {
    return colors::YELLOW;
  }
  if (TITANE_NAME == name)
  {
    return colors::PURPLE;
  }

  throw std::invalid_argument("Unsupported resource name " + name);
}

auto colorFromFaction(const bsgo::Faction &faction) -> Color
{
  switch (faction)
  {
    case bsgo::Faction::COLONIAL:
      return Color{.rgb = RgbData{153, 193, 241}};
    case bsgo::Faction::CYLON:
      return Color{.rgb = RgbData{191, 64, 64}};
    default:
      throw std::invalid_argument("Unsupported faction " + bsgo::str(faction));
  }
}

} // namespace pge
