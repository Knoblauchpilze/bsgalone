
#pragma once

#include "AbstractRepository.hh"
#include "Faction.hh"
#include "ShipClass.hh"
#include "Slot.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <set>
#include <unordered_set>

namespace bsgo {

struct PlayerShip
{
  Uuid id{};
  Faction faction{};
  ShipClass shipClass{};
  Uuid ship{};
  std::string name{};
  std::optional<Uuid> player{};
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

  std::optional<Uuid> system{};
  bool docked{};

  core::Duration jumpTime{};
  core::Duration jumpTimeInThreat{};
  std::optional<Uuid> jumpSystem{};

  std::unordered_map<Slot, int> slots{};
};

class PlayerShipRepository : public AbstractRepository
{
  public:
  PlayerShipRepository(const DbConnectionShPtr &connection);
  ~PlayerShipRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid ship) const -> PlayerShip;
  auto findOneByPlayerAndActive(const Uuid player) const -> PlayerShip;
  auto findAllByPlayer(const Uuid player) const -> std::unordered_set<Uuid>;
  auto findAllAvailableWeaponSlotByShip(const Uuid ship) -> std::set<Uuid>;

  void save(const PlayerShip &ship);

  private:
  auto fetchShipBase(const Uuid ship) const -> PlayerShip;
  void fetchSlots(const Uuid ship, PlayerShip &out) const;
  void registerShipJump(const Uuid ship, const Uuid system) const;
  void cancelShipJump(const Uuid ship) const;
};

using PlayerShipRepositoryShPtr = std::shared_ptr<PlayerShipRepository>;

} // namespace bsgo
