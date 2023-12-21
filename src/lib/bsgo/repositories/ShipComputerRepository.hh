
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_set>

namespace bsgo {

class ShipComputerRepository : public AbstractRepository
{
  public:
  ShipComputerRepository(const DbConnectionShPtr &connection);
  ~ShipComputerRepository() override = default;

  void initialize() override;

  bool existByShipAndComputer(const Uuid &ship, const Uuid &computer) const;
  auto findAllByShip(const Uuid &ship) const -> std::unordered_set<Uuid>;
};

using ShipComputerRepositoryShPtr = std::shared_ptr<ShipComputerRepository>;

} // namespace bsgo
