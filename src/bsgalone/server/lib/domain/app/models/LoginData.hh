
#pragma once

#include "ClientId.hh"
#include "GameRole.hh"
#include <string>

namespace bsgalone::core {

struct LoginData
{
  std::string username{};
  std::string password{};
  GameRole role{};
  net::ClientId clientId{};
};

} // namespace bsgalone::core
