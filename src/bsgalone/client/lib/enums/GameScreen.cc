
#include "GameScreen.hh"

namespace bsgalone::client {

auto str(const GameScreen s) noexcept -> std::string
{
  switch (s)
  {
    case GameScreen::LOGIN:
      return "login";
    case GameScreen::LOADING:
      return "loading";
    case GameScreen::GAME:
      return "game";
    case GameScreen::EXIT:
      return "exit";
    default:
      return "unknown";
  }
}

} // namespace bsgalone::client
