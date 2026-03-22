
#pragma once

#include "AbstractService.hh"
#include "LoginData.hh"
#include <memory>

namespace bsgalone::server {

class LoginService : public AbstractService
{
  public:
  LoginService(const core::Repositories &repositories);
  ~LoginService() override = default;

  auto tryLogin(const core::LoginData &data) const -> std::optional<core::Uuid>;

  auto getPlayerSystemDbId(const core::Uuid playerDbId) const -> core::Uuid;
};

using LoginServicePtr = std::unique_ptr<LoginService>;

} // namespace bsgalone::server
