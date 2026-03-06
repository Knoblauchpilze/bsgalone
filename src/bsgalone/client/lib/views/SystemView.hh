
#pragma once

#include "AbstractView.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "PlayerData.hh"
#include "PlayerShipData.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace bsgalone::client {

class SystemView : public AbstractView
{
  public:
  SystemView(core::CoordinatorShPtr coordinator, const core::DatabaseEntityMapper &entityMapper);
  ~SystemView() override = default;

  bool isReady() const noexcept override;
  void reset() override;

  auto getAsteroidsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>;
  auto getOutpostsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>;
  auto getBulletsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>;

  auto getAsteroid(const core::Uuid asteroidDbId) const -> core::Entity;

  auto getPlayer(const core::Uuid playerDbId) const -> core::PlayerData;

  auto getSystemPlayers() const -> std::vector<core::PlayerData>;
  auto getSystemShips() const -> std::vector<core::PlayerShipData>;

  protected:
  void handleMessageInternal(const core::IMessage &message) override;

  private:
  core::CoordinatorShPtr m_coordinator{};
  const core::DatabaseEntityMapper &m_entityMapper;

  std::vector<core::PlayerData> m_players{};
  std::vector<core::PlayerShipData> m_playerShips{};
};

using SystemViewShPtr = std::shared_ptr<SystemView>;

} // namespace bsgalone::client
