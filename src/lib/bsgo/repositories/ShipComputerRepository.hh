
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

class ShipComputerRepository : public IRepository
{
  public:
  ShipComputerRepository();
  ~ShipComputerRepository() override = default;

  auto findAllById(const Uuid &ship) const -> std::vector<Uuid>;
};

using ShipComputerRepositoryShPtr = std::shared_ptr<ShipComputerRepository>;

} // namespace bsgo
