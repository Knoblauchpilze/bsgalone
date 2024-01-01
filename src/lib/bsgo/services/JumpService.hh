
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class JumpService : public AbstractService
{
  public:
  JumpService(const Repositories &repositories, const CoordinatorShPtr &coordinator);
  ~JumpService() override = default;

  bool tryRegisterJump(const Uuid &shipId, const Uuid &system) const;
  bool tryCancelJump(const Uuid &shipId) const;
  bool tryJump(const Uuid &shipId) const;
};

using JumpServiceShPtr = std::shared_ptr<JumpService>;

} // namespace bsgo
