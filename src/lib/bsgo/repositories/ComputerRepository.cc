

#include "ComputerRepository.hh"

namespace bsgo {

ComputerRepository::ComputerRepository()
  : IRepository("computer")
{}

auto ComputerRepository::findOneById(const Uuid & /*computer*/) const -> Computer
{
  Computer out;
  out.powerCost = 20.0f;

  out.range = 4.1f;

  constexpr auto RELOAD_TIME_IN_MS = 10000;
  out.reloadTime                   = utils::Milliseconds(RELOAD_TIME_IN_MS);

  return out;
}

} // namespace bsgo
