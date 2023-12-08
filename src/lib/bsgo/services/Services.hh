
#pragma once

#include "Repositories.hh"

namespace bsgo {

struct Services
{};

auto createServices(const Repositories &repositories) -> Services;

} // namespace bsgo
