
#include "ShipComputerRepository.hh"

namespace bsgo {

ShipComputerRepository::ShipComputerRepository()
  : IRepository("computer")
{
  addModule("ship");
}

auto ShipComputerRepository::findAllByShip(const Uuid &ship) const -> std::unordered_set<Uuid>
{
  switch (ship)
  {
    case Uuid{0}:
      return {Uuid(0), Uuid(1)};
    case Uuid{1}:
      return {};
    case Uuid{2}:
      return {};
    default:
      error("Ship " + str(ship) + " not found");
      break;
  }

  return {};
}

} // namespace bsgo
