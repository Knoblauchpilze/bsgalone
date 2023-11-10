
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace bsgo {

struct Asteroid
{
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};
  float health{0.0f};
  float radius{0.5f};

  bool loot{false};
};

class AsteroidRepository : public AbstractRepository
{
  public:
  AsteroidRepository(const DbConnectionShPtr &connection);
  ~AsteroidRepository() override = default;

  auto findOneById(const Uuid &asteroid) const -> Asteroid;
};

using AsteroidRepositoryShPtr = std::shared_ptr<AsteroidRepository>;

} // namespace bsgo
