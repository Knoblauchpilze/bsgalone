
#pragma once

#include "AbstractRepository.hh"
#include "Faction.hh"
#include "ShipClass.hh"
#include "Slot.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgo {

struct PlayerShip
{
  Faction faction{};
  ShipClass shipClass{};
  std::optional<Uuid> player{};
  std::string name{};
  bool active{};

  float hullPoints{0.0f};
  float maxHullPoints{0.0f};
  float hullPointsRegen{0.0f};

  float powerPoints{0.0f};
  float maxPowerPoints{0.0f};
  float powerRegen{0.0f};

  float acceleration{5.0f};
  float speed{4.0f};

  float radius{0.5f};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};

  utils::Duration jumpTime{};
  utils::Duration jumpTimeInThreat{};

  std::unordered_map<Slot, int> slots{};
};

class PlayerShipRepository : public AbstractRepository
{
  public:
  PlayerShipRepository(const DbConnectionShPtr &connection);
  ~PlayerShipRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid &ship) const -> PlayerShip;
  auto findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>;

  private:
  auto fetchShipBase(const Uuid &ship) const -> PlayerShip;
  void fetchSlots(const Uuid &ship, PlayerShip &out) const;
};

using PlayerShipRepositoryShPtr = std::shared_ptr<PlayerShipRepository>;

} // namespace bsgo
