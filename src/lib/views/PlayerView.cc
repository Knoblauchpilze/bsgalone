
#include "PlayerView.hh"

namespace bsgo {

PlayerView::PlayerView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : IView("player", coordinator, repositories)
{}

} // namespace bsgo
