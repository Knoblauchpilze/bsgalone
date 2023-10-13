
#include "SystemRepository.hh"

namespace bsgo {

SystemRepository::SystemRepository()
  : IRepository("system")
{}

auto SystemRepository::findAllAsteroidsBySystem(const Uuid & /*system*/) const -> std::vector<Uuid>
{
  return {Uuid(0), Uuid(1), Uuid(2), Uuid(3), Uuid(4)};
}

auto SystemRepository::findAllShipsBySystem(const Uuid & /*system*/) const
  -> std::vector<SystemShip>
{
  return {SystemShip{Uuid(0), Uuid(0)}};
}

auto SystemRepository::findAllOutpostsBySystem(const Uuid & /*system*/) const -> std::vector<Uuid>
{
  return {Uuid(0)};
}

} // namespace bsgo
