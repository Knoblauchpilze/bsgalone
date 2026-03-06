
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include <memory>

namespace bsgalone::server {

class JumpService : public AbstractService
{
  public:
  JumpService(const core::Repositories &repositories,
              core::CoordinatorShPtr coordinator,
              const core::DatabaseEntityMapper &entityMapper);
  ~JumpService() override = default;

  bool tryRegisterJump(const core::Uuid shipDbId, const core::Uuid system) const;
  bool tryCancelJump(const core::Uuid shipDbId) const;

  private:
  core::CoordinatorShPtr m_coordinator{};
  const core::DatabaseEntityMapper &m_entityMapper;
};

using JumpServiceShPtr = std::shared_ptr<JumpService>;

} // namespace bsgalone::server
