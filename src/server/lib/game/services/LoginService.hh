
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class LoginService : public AbstractService
{
  public:
  LoginService(const Repositories &repositories);
  ~LoginService() override = default;

  auto tryLogin(const std::string &name, const std::string &password) const -> std::optional<Uuid>;

  auto getPlayerSystemDbId(const Uuid playerDbId) const -> Uuid;
};

using LoginServicePtr   = std::unique_ptr<LoginService>;
using LoginServiceShPtr = std::shared_ptr<LoginService>;

} // namespace bsgo
