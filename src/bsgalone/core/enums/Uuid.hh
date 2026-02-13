
#pragma once

#include <string>

namespace bsgo {

using Uuid = int;

auto str(const Uuid id) -> std::string;
auto toDbId(const Uuid id) -> int;
auto fromDbId(const int dbId) -> Uuid;

} // namespace bsgo
