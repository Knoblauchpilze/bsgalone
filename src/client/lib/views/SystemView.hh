
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

namespace pge {

class SystemView : public AbstractView
{
  public:
  SystemView(bsgo::CoordinatorShPtr coordinator, const bsgo::DatabaseEntityMapper &entityMapper);
  ~SystemView() override = default;

  bool isReady() const noexcept override;
  void reset() override;

  auto getAsteroidsWithin(const bsgo::IBoundingBox &bbox) const -> std::vector<bsgo::Entity>;
  auto getOutpostsWithin(const bsgo::IBoundingBox &bbox) const -> std::vector<bsgo::Entity>;
  auto getBulletsWithin(const bsgo::IBoundingBox &bbox) const -> std::vector<bsgo::Entity>;

  auto getAsteroid(const bsgo::Uuid asteroidDbId) const -> bsgo::Entity;

  auto getPlayer(const bsgo::Uuid playerDbId) const -> bsgo::PlayerData;

  auto getSystemPlayers() const -> std::vector<bsgo::PlayerData>;
  auto getSystemShips() const -> std::vector<bsgo::PlayerShipData>;

  protected:
  void handleMessageInternal(const bsgalone::core::IMessage &message) override;

  private:
  bsgo::CoordinatorShPtr m_coordinator{};
  const bsgo::DatabaseEntityMapper &m_entityMapper;

  std::vector<bsgo::PlayerData> m_players{};
  std::vector<bsgo::PlayerShipData> m_playerShips{};
};

using SystemViewShPtr = std::shared_ptr<SystemView>;

} // namespace pge
