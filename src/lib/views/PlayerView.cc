
#include "PlayerView.hh"

namespace bsgo {

PlayerView::PlayerView(const CoordinatorShPtr &coordinator)
  : IView("player", coordinator)
{}

} // namespace bsgo
