
#pragma once

#include "AbstractRepository.hh"
#include "Faction.hh"
#include "ShipClass.hh"
#include "Slot.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

struct Ship
{
  Uuid id{};
  Faction faction{};
  ShipClass shipClass{};
  std::string name{};

  float maxHullPoints{0.0f};
  float hullPointsRegen{0.0f};

  float maxPowerPoints{0.0f};
  float powerRegen{0.0f};

  float acceleration{5.0f};
  float speed{4.0f};

  float radius{0.5f};

  core::Duration jumpTime{};
  core::Duration jumpTimeInThreat{};

  std::unordered_map<Slot, int> slots{};
};

class ShipRepository : public AbstractRepository
{
  public:
  ShipRepository(const DbConnectionShPtr &connection);
  ~ShipRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid ship) const -> Ship;
  auto findAllByFaction(const Faction &faction) const -> std::vector<Ship>;
  auto findOneByFactionAndStarting(const Faction &faction, const bool startingShip) const -> Ship;

  private:
  auto fetchShipBase(const Uuid ship) const -> Ship;
  void fetchSlots(const Uuid ship, Ship &out) const;
};

using ShipRepositoryShPtr = std::shared_ptr<ShipRepository>;

} // namespace bsgo
