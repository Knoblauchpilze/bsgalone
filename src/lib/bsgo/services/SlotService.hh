
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class SlotService : public AbstractService
{
  public:
  SlotService(const Repositories &repositories, const CoordinatorShPtr &coordinator);
  ~SlotService() override = default;

  bool tryToggleWeapon(const Uuid &shipEntityId, const int weaponIndex) const;
  bool tryToggleComputer(const Uuid &shipEntityId, const int computerIndex) const;
};

using SlotServiceShPtr = std::shared_ptr<SlotService>;

} // namespace bsgo
