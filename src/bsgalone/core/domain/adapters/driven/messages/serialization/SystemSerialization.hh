
#pragma once

#include "System.hh"

namespace bsgalone::core {

auto operator<<(std::ostream &out, const System &data) -> std::ostream &;
bool operator>>(std::istream &in, System &data);

} // namespace bsgalone::core
