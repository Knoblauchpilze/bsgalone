
#pragma once

#include "AbstractRepository.hh"
#include "Tick.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace bsgo {

struct Asteroid
{
  Uuid id{};
  Uuid system{};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};
  float health{0.0f};
  float maxHealth{0.0f};
  float radius{0.5f};

  bool loot{false};
};

class AsteroidRepository : public AbstractRepository
{
  public:
  AsteroidRepository(const DbConnectionShPtr &connection);
  ~AsteroidRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid asteroid) const -> Asteroid;
  auto findAllByRespawnTimeUntil(const chrono::Tick &until) -> std::vector<Asteroid>;

  void save(const Asteroid &asteroid);

  void saveRespawn(const Uuid asteroid, const chrono::Tick &death, const chrono::Tick &respawn);
  void deleteRespawn(const Uuid asteroid);

  private:
  auto fetchAsteroidBase(const Uuid asteroid) const -> Asteroid;
  void fetchLoot(const Uuid asteroid, Asteroid &out) const;
};

using AsteroidRepositoryShPtr = std::shared_ptr<AsteroidRepository>;

} // namespace bsgo
