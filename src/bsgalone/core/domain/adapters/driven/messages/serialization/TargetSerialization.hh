
#pragma once

#include "Target.hh"

namespace bsgalone::core {

auto operator<<(std::ostream &out, const Target &data) -> std::ostream &;
bool operator>>(std::istream &in, Target &data);

} // namespace bsgalone::core
