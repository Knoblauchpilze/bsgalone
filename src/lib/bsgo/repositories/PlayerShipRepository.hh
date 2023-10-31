
#pragma once

#include "Faction.hh"
#include "IRepository.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace bsgo {

struct PlayerShip
{
  Faction faction;
  std::optional<Uuid> player{};

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

  std::vector<Uuid> weapons{};
  std::vector<Uuid> computers{};
};

class PlayerShipRepository : public IRepository
{
  public:
  PlayerShipRepository();
  ~PlayerShipRepository() override = default;

  auto findOneById(const Uuid &player) const -> PlayerShip;
};

using PlayerShipRepositoryShPtr = std::shared_ptr<PlayerShipRepository>;

} // namespace bsgo
