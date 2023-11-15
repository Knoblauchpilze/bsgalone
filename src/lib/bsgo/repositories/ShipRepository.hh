
#pragma once

#include "AbstractRepository.hh"
#include "Faction.hh"
#include "Slot.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

struct Ship
{
  Faction faction{};
  std::string name{};

  float maxHullPoints{0.0f};
  float hullPointsRegen{0.0f};

  float maxPowerPoints{0.0f};
  float powerRegen{0.0f};

  float acceleration{5.0f};
  float speed{4.0f};

  float radius{0.5f};

  std::unordered_map<Slot, int> slots{};
};

class ShipRepository : public AbstractRepository
{
  public:
  ShipRepository(const DbConnectionShPtr &connection);
  ~ShipRepository() override = default;

  auto findOneById(const Uuid &ship) const -> Ship;

  private:
  auto fetchShipBase(const Uuid &ship) const -> Ship;
  void fetchSlots(const Uuid &ship, Ship &out) const;
};

using ShipRepositoryShPtr = std::shared_ptr<ShipRepository>;

} // namespace bsgo
