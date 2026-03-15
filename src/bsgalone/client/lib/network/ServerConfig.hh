
#pragma once

#include "GameRole.hh"
#include <optional>
#include <string>

namespace bsgalone::client {

struct User
{
  std::string name{};
  std::string password{};
  core::GameRole role{};
};

struct ServerConfig
{
  int port{};
  std::optional<User> autoConnect{};
};

} // namespace bsgalone::client
