
#pragma once

#include "Uuid.hh"
#include <string>

namespace bsgalone::server {

struct Account
{
  core::Uuid dbId{};
  std::string username{};
  std::string password{};

  bool operator==(const Account &rhs) const;
};

} // namespace bsgalone::server
