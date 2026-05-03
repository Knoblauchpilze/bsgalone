
#pragma once

#include <string>

namespace bsgalone::core {

using EntityId = int;

auto str(const EntityId id) -> std::string;

} // namespace bsgalone::core