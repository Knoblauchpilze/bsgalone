
#pragma once

#include "AbstractService.hh"
#include <memory>
#include <optional>

namespace bsgo {

class ShipService : public AbstractService
{
  public:
  ShipService(const Repositories &repositories);
  ~ShipService() override = default;

  bool trySelectShip(const Uuid &playerShipId) const;
};

using ShipServiceShPtr = std::shared_ptr<ShipService>;

} // namespace bsgo
