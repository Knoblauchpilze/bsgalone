
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_set>

namespace bsgo {

class ShipComputerRepository : public IRepository
{
  public:
  ShipComputerRepository();
  ~ShipComputerRepository() override = default;

  auto findAllByShip(const Uuid &ship) const -> std::unordered_set<Uuid>;
};

using ShipComputerRepositoryShPtr = std::shared_ptr<ShipComputerRepository>;

} // namespace bsgo
