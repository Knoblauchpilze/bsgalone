
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include <memory>

namespace bsgo {

class SlotService : public AbstractService
{
  public:
  SlotService(const Repositories &repositories, CoordinatorShPtr coordinator);
  ~SlotService() override = default;

  bool tryToggleWeapon(const Uuid shipEntityId, const int weaponIndex) const;
  bool tryToggleComputer(const Uuid shipEntityId, const int computerIndex) const;

  struct SlotUpdateData
  {
    std::optional<utils::Duration> elapsedSinceLastFired{};
  };
  bool trySyncComputer(const Uuid shipEntityId,
                       const int computerIndex,
                       const SlotUpdateData &data) const;

  private:
  CoordinatorShPtr m_coordinator{};
};

using SlotServiceShPtr = std::shared_ptr<SlotService>;

} // namespace bsgo
