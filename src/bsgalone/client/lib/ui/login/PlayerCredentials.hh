
#pragma once

#include <string>

namespace bsgalone::client {

struct PlayerCredentials
{
  std::string username{};
  std::string password{};

  bool valid() const;
};

} // namespace bsgalone::client
