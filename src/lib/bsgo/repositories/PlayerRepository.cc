
#include "PlayerRepository.hh"

namespace bsgo {

PlayerRepository::PlayerRepository()
  : IRepository("player")
{}

auto PlayerRepository::findSystemById(const Uuid & /*id*/) const -> Uuid
{
  return {};
}

auto PlayerRepository::findShipById(const Uuid & /*id*/) const -> Uuid
{
  return {};
}

} // namespace bsgo
