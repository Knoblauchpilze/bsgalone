
#include "ShipClass.hh"
#include <stdexcept>

namespace bsgo {

auto str(const ShipClass shipClass) -> std::string
{
  switch (shipClass)
  {
    case ShipClass::STRIKE:
      return "strike";
    case ShipClass::LINE:
      return "line";
    default:
      return "unknown";
  }
}

auto toDbShipClass(const ShipClass shipClass) -> std::string
{
  return str(shipClass);
}

auto fromDbShipClass(const std::string &dbShipClass) -> ShipClass
{
  if ("strike" == dbShipClass)
  {
    return ShipClass::STRIKE;
  }
  if ("line" == dbShipClass)
  {
    return ShipClass::LINE;
  }

  throw std::invalid_argument("Unknown ship class \"" + dbShipClass + "\"");
}

} // namespace bsgo
