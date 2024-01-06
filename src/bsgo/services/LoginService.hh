
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class LoginService : public AbstractService
{
  public:
  LoginService(const Repositories &repositories, const CoordinatorShPtr &coordinator);
  ~LoginService() override = default;

  auto tryLogin(const std::string &name, const std::string &password) const -> std::optional<Uuid>;
};

using LoginServiceShPtr = std::shared_ptr<LoginService>;

} // namespace bsgo
