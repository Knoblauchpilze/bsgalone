
#include "PlayerRepository.hh"

namespace bsgo {

PlayerRepository::PlayerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{}

auto PlayerRepository::findSystemById(const Uuid & /*player*/) const -> Uuid
{
  return {};
}

} // namespace bsgo
