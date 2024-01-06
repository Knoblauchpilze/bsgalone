
#pragma once

#include <string>

namespace bsgo {

enum class ShipClass
{
  STRIKE,
  LINE
};

auto str(const ShipClass shipClass) -> std::string;
auto toDbShipClass(const ShipClass shipClass) -> std::string;
auto fromDbShipClass(const std::string &dbShipClass) -> ShipClass;

} // namespace bsgo
