
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

class PlayerRepository : public AbstractRepository
{
  public:
  PlayerRepository(const DbConnectionShPtr &connection);
  ~PlayerRepository() override = default;

  auto findSystemById(const Uuid &player) const -> Uuid;
};

using PlayerRepositoryShPtr = std::shared_ptr<PlayerRepository>;

} // namespace bsgo
