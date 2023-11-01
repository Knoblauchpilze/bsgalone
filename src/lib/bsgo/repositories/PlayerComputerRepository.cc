
#include "PlayerComputerRepository.hh"

namespace bsgo {

PlayerComputerRepository::PlayerComputerRepository()
  : IRepository("computer")
{
  addModule("player");
}

auto PlayerComputerRepository::findAllById(const Uuid & /*player*/) const -> std::vector<Uuid>
{
  return {};
}

} // namespace bsgo
