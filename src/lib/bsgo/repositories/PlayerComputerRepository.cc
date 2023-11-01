
#include "PlayerComputerRepository.hh"

namespace bsgo {

PlayerComputerRepository::PlayerComputerRepository()
  : IRepository("computer")
{
  addModule("player");
}

auto PlayerComputerRepository::findAllById(const Uuid &player) const -> std::vector<Uuid>
{
  if (player != Uuid{0})
  {
    error("Player " + str(player) + " not found");
  }
  return {};
}

} // namespace bsgo
