
#include "GameColorUtils.hh"

namespace pge {
constexpr auto TYLIUM_NAME = "tylium";
constexpr auto TITANE_NAME = "titane";

auto colorFromResourceName(const std::string &name) -> olc::Pixel
{
  if (TYLIUM_NAME == name)
  {
    return olc::YELLOW;
  }
  if (TITANE_NAME == name)
  {
    return olc::PURPLE;
  }

  throw std::invalid_argument("Unsupported resource name " + name);
}

} // namespace pge
