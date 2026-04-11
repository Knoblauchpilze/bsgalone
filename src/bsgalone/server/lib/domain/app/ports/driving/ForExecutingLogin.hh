
#pragma once

#include "LoginData.hh"
#include <memory>

namespace bsgalone::server {

class ForExecutingLogin
{
  public:
  ForExecutingLogin()          = default;
  virtual ~ForExecutingLogin() = default;

  virtual void performLogin(const LoginData &data) = 0;
};

using ForExecutingLoginPtr = std::unique_ptr<ForExecutingLogin>;

} // namespace bsgalone::server
