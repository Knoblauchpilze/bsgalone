
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

  auto getUuid() const noexcept -> Uuid;

  auto getHealth() const noexcept -> float;
  auto getMaxHealth() const noexcept -> float;
  auto getPower() const noexcept -> float;
  auto getMaxPower() const noexcept -> float;

  bool hasTarget() const noexcept;
  void setTarget(const Uuid &uuid);
  void clearTarget();

  auto getTargetHealth() const noexcept -> float;
  auto getTargetMaxHealth() const noexcept -> float;
  auto getTargetPower() const noexcept -> float;
  auto getTargetMaxPower() const noexcept -> float;

  private:
  Uuid m_playerId{};
  PlayerRepositoryShPtr m_playerRepo;
  PlayerShipRepositoryShPtr m_playerShipRepo;
  std::optional<Uuid> m_target{};
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
