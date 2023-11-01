
#include "PlayerView.hh"

namespace bsgo {

PlayerView::PlayerView(const Uuid &playerId,
                       const CoordinatorShPtr &coordinator,
                       const Repositories &repositories)
  : IView("player", coordinator, repositories)
  , m_playerId(playerId)
{}

auto PlayerView::getPlayerResources() const -> std::vector<Resource>
{
  return m_repositories.playerResourceRepository->findAllByPlayer(m_playerId);
}

} // namespace bsgo
