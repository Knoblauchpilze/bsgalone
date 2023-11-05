
#pragma once

#include "Faction.hh"
#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

struct Ship
{
  Faction faction;

  float maxHullPoints{0.0f};
  float hullPointsRegen{0.0f};

  float maxPowerPoints{0.0f};
  float powerRegen{0.0f};

  float acceleration{5.0f};
  float speed{4.0f};

  float radius{0.5f};
};

class ShipRepository : public IRepository
{
  public:
  ShipRepository();
  ~ShipRepository() override = default;

  auto findOneById(const Uuid &ship) const -> Ship;
};

using ShipRepositoryShPtr = std::shared_ptr<ShipRepository>;

} // namespace bsgo
