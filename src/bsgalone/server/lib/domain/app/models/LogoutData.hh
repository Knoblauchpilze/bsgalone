
#pragma once

#include "Uuid.hh"

namespace bsgalone::server {

struct LogoutData
{
  core::Uuid playerDbId{};
};

} // namespace bsgalone::server
