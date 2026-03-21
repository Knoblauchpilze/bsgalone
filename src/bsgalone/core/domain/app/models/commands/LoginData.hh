
#pragma once

#include "GameRole.hh"
#include <string>

namespace bsgalone::core {

struct LoginData
{
  std::string username{};
  std::string password{};
  GameRole role{};
};

} // namespace bsgalone::core
