
#pragma once

#include "IView.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <optional>

namespace bsgo {

class ShipView : public utils::CoreObject, public IView
{
  public:
  ShipView(const Repositories &repositories);
  ~ShipView() override = default;

  void update(const float elapsedSeconds) override;

  auto getPlayerShipId() const noexcept -> Uuid;
  auto getPlayerShip(const Uuid &player) const -> PlayerShip;

  private:
  Uuid m_playerId{};
  PlayerRepositoryShPtr m_playerRepo;
  PlayerShipRepositoryShPtr m_playerShipRepo;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
