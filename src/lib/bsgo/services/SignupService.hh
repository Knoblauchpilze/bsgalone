
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class SignupService : public AbstractService
{
  public:
  SignupService(const Repositories &repositories);
  ~SignupService() override = default;
};

using SignupServiceShPtr = std::shared_ptr<SignupService>;

} // namespace bsgo
