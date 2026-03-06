
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include <memory>

namespace bsgalone::server {

class SlotService : public AbstractService
{
  public:
  SlotService(const core::Repositories &repositories,
              core::CoordinatorShPtr coordinator,
              const core::DatabaseEntityMapper &entityMapper);
  ~SlotService() override = default;

  struct TogglingResult
  {
    bool success{false};
    bool active{false};
  };
  auto tryToggleWeapon(const core::Uuid shipDbId, const core::Uuid weaponDbId) const
    -> TogglingResult;
  bool tryToggleComputer(const core::Uuid shipDbId, const core::Uuid computerDbId) const;

  private:
  core::CoordinatorShPtr m_coordinator{};
  const core::DatabaseEntityMapper &m_entityMapper;
};

using SlotServiceShPtr = std::shared_ptr<SlotService>;

} // namespace bsgalone::server
