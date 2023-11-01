
#pragma once

#include "IView.hh"
#include <memory>

namespace bsgo {

class PlayerView : public IView
{
  public:
  PlayerView(const Uuid &playerId,
             const CoordinatorShPtr &coordinator,
             const Repositories &repositories);
  ~PlayerView() override = default;

  auto getPlayerResources() const -> std::vector<Resource>;

  private:
  Uuid m_playerId;
};

using PlayerViewShPtr = std::shared_ptr<PlayerView>;

} // namespace bsgo
