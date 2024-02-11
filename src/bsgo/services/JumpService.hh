
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include <memory>

namespace bsgo {

class JumpService : public AbstractService
{
  public:
  JumpService(const Repositories &repositories,
              CoordinatorShPtr coordinator,
              const DatabaseEntityMapper &entityMapper);
  ~JumpService() override = default;

  bool tryRegisterJump(const Uuid shipDbId, const Uuid system) const;
  bool tryCancelJump(const Uuid shipDbId) const;

  private:
  CoordinatorShPtr m_coordinator{};
  const DatabaseEntityMapper &m_entityMapper;
};

using JumpServiceShPtr = std::shared_ptr<JumpService>;

} // namespace bsgo
