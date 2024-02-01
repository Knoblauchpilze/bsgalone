
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include <memory>

namespace bsgo {

class JumpService : public AbstractService
{
  public:
  JumpService(const Repositories &repositories, CoordinatorShPtr coordinator);
  ~JumpService() override = default;

  bool tryRegisterJump(const Uuid shipDbId, const Uuid shipEntityId, const Uuid system) const;
  bool tryCancelJump(const Uuid shipDbId, const Uuid shipEntityId) const;
  bool tryJump(const Uuid shipDbId, const Uuid shipEntityId) const;

  private:
  CoordinatorShPtr m_coordinator{};
};

using JumpServiceShPtr = std::shared_ptr<JumpService>;

} // namespace bsgo
