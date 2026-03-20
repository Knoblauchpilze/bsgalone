
#pragma once

#include "GameRole.hh"
#include <memory>
#include <string>

namespace bsgalone::core {

class ForExecutingLogin
{
  public:
  ForExecutingLogin()          = default;
  virtual ~ForExecutingLogin() = default;

  struct Data
  {
    std::string username{};
    std::string password{};
    GameRole role{};
  };
  virtual void performLogin(const Data &data) = 0;
};

using ForExecutingLoginPtr = std::unique_ptr<ForExecutingLogin>;

} // namespace bsgalone::core
