
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class LoginService : public AbstractService
{
  public:
  LoginService(const Repositories &repositories);
  ~LoginService() override = default;

  struct LoginData
  {
    std::string name{};
    std::string password{};
    GameRole role{};
  };
  auto tryLogin(const LoginData &data) const -> std::optional<Uuid>;

  auto getPlayerSystemDbId(const Uuid playerDbId) const -> Uuid;
};

using LoginServicePtr = std::unique_ptr<LoginService>;

} // namespace bsgo
