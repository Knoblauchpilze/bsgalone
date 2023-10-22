

#include "ComputerRepository.hh"

namespace bsgo {

ComputerRepository::ComputerRepository()
  : IRepository("computer")
{}

auto ComputerRepository::findOneById(const Uuid & /*computer*/) const -> Computer
{
  Computer out;
  out.powerCost                    = 20.0f;
  out.range                        = 4.1f;
  constexpr auto RELOAD_TIME_IN_MS = 10000;
  out.reloadTime                   = utils::Milliseconds(RELOAD_TIME_IN_MS);

  constexpr auto DURATION_IN_MS = 1500;
  out.duration                  = {utils::Milliseconds(DURATION_IN_MS)};
  out.damageModifier            = {1.5f};

  return out;
}

} // namespace bsgo
