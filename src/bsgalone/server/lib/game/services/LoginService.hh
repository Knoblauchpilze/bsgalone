
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgalone::server {

class LoginService : public AbstractService
{
  public:
  LoginService(const core::Repositories &repositories);
  ~LoginService() override = default;

  struct LoginData
  {
    std::string name{};
    std::string password{};
    core::GameRole role{};
  };
  auto tryLogin(const LoginData &data) const -> std::optional<core::Uuid>;

  auto getPlayerSystemDbId(const core::Uuid playerDbId) const -> core::Uuid;
};

using LoginServicePtr = std::unique_ptr<LoginService>;

} // namespace bsgalone::server
