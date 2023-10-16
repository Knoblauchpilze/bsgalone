
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class PlayerRepository : public IRepository
{
  public:
  PlayerRepository();
  ~PlayerRepository() override = default;

  auto findSystemById(const Uuid &player) const -> Uuid;
  auto findShipById(const Uuid &player) const -> Uuid;
};

} // namespace bsgo
