
#include "PlayerRepository.hh"

namespace bsgo {

PlayerRepository::PlayerRepository()
  : IRepository("player")
{}

auto PlayerRepository::findSystemById(const Uuid & /*player*/) const -> Uuid
{
  return {};
}

auto PlayerRepository::findShipById(const Uuid & /*player*/) const -> Uuid
{
  return {};
}

} // namespace bsgo
