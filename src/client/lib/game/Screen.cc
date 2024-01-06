
#include "Screen.hh"

namespace pge {

auto str(const Screen s) noexcept -> std::string
{
  switch (s)
  {
    case Screen::LOGIN:
      return "login";
    case Screen::GAME:
      return "game";
    case Screen::MAP:
      return "map";
    case Screen::OUTPOST:
      return "outpost";
    case Screen::EXIT:
      return "exit";
    default:
      return "unknown";
  }
}

} // namespace pge
