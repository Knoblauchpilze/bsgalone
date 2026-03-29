
#pragma once

#include "Uuid.hh"
#include <string>

namespace bsgalone::core {

struct Account
{
  Uuid dbId{};
  std::string username{};
  std::string password{};

  bool operator==(const Account &rhs) const;
};

} // namespace bsgalone::core
