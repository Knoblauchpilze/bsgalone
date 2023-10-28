
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

class PlayerRepository : public IRepository
{
  public:
  PlayerRepository();
  ~PlayerRepository() override = default;

  auto findSystemById(const Uuid &player) const -> Uuid;
  auto findShipById(const Uuid &player) const -> Uuid;
};

using PlayerRepositoryShPtr = std::shared_ptr<PlayerRepository>;

} // namespace bsgo
