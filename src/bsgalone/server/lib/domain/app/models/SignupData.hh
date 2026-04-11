
#pragma once

#include "ClientId.hh"
#include "Faction.hh"
#include <string>

namespace bsgalone::server {

struct SignupData
{
  std::string username{};
  std::string password{};
  core::Faction faction{};
  net::ClientId clientId{};
};

} // namespace bsgalone::server
