
#pragma once

#include "LogoutData.hh"
#include <memory>

namespace bsgalone::server {

class ForExecutingLogout
{
  public:
  ForExecutingLogout()          = default;
  virtual ~ForExecutingLogout() = default;

  virtual void performLogout(const LogoutData &data) = 0;
};

using ForExecutingLogoutPtr = std::unique_ptr<ForExecutingLogout>;

} // namespace bsgalone::server
