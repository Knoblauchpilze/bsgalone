
#pragma once

#include "ClientId.hh"
#include "GameRole.hh"
#include <string>

namespace bsgalone::server {

struct LoginData
{
  std::string username{};
  std::string password{};
  core::GameRole role{};
  net::ClientId clientId{};
};

} // namespace bsgalone::server
