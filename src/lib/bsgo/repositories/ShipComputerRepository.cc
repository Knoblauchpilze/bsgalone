
#include "ShipComputerRepository.hh"

namespace bsgo {

ShipComputerRepository::ShipComputerRepository()
  : IRepository("computer")
{
  addModule("ship");
}

auto ShipComputerRepository::findAllById(const Uuid &ship) const -> std::vector<Uuid>
{
  if (ship != Uuid{0})
  {
    error("Ship " + str(ship) + " not found");
  }
  return {Uuid(0), Uuid(1)};
}

} // namespace bsgo
