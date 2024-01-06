
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_map>

namespace bsgo {

class ShipPriceRepository : public AbstractRepository
{
  public:
  ShipPriceRepository(const DbConnectionShPtr &connection);
  ~ShipPriceRepository() override = default;

  void initialize() override;

  auto findAllByShip(const Uuid &ship) const -> std::unordered_map<Uuid, float>;
};

using ShipPriceRepositoryShPtr = std::shared_ptr<ShipPriceRepository>;

} // namespace bsgo
