
#include "ShipComputerRepository.hh"

namespace bsgo {

ShipComputerRepository::ShipComputerRepository()
  : IRepository("computer")
{
  addModule("ship");
}

auto ShipComputerRepository::findAllById(const Uuid & /*ship*/) const -> std::vector<Uuid>
{
  return {};
}

} // namespace bsgo
