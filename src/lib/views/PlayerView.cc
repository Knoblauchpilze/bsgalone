
#include "PlayerView.hh"

namespace bsgo {

PlayerView::PlayerView()
  : utils::CoreObject("player")
{
  setService("view");
}

void PlayerView::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
