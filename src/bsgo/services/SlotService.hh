
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include <memory>

namespace bsgo {

class SlotService : public AbstractService
{
  public:
  SlotService(const Repositories &repositories,
              CoordinatorShPtr coordinator,
              const DatabaseEntityMapper &entityMapper);
  ~SlotService() override = default;

  bool tryToggleWeapon(const Uuid shipDbId, const Uuid weaponDbId) const;
  bool tryToggleComputer(const Uuid shipDbId, const Uuid computerDbId) const;

  private:
  CoordinatorShPtr m_coordinator{};
  const DatabaseEntityMapper &m_entityMapper;
};

using SlotServiceShPtr = std::shared_ptr<SlotService>;

} // namespace bsgo
