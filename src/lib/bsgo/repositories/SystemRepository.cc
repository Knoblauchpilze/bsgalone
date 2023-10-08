
#include "SystemRepository.hh"

namespace bsgo {

SystemRepository::SystemRepository()
  : IRepository("system")
{}

auto SystemRepository::findAsteroids(const Uuid & /*system*/) -> std::vector<Uuid>
{
  return {Uuid(0), Uuid(1), Uuid(2), Uuid(3), Uuid(4)};
}

auto SystemRepository::findShips(const Uuid & /*system*/) -> std::vector<Uuid>
{
  return {{}};
}

} // namespace bsgo
