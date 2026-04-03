
#pragma once

#include "ClientId.hh"
#include "Faction.hh"
#include <string>

namespace bsgalone::core {

struct SignupData
{
  std::string username{};
  std::string password{};
  Faction faction{};
  net::ClientId clientId{};
};

} // namespace bsgalone::core
