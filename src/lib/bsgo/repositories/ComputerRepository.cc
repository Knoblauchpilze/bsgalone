

#include "ComputerRepository.hh"

namespace bsgo {

ComputerRepository::ComputerRepository()
  : IRepository("computer")
{}

auto ComputerRepository::findOneById(const Uuid &computer) const -> Computer
{
  error("Computer " + str(computer) + " not found");
  return {};
}

} // namespace bsgo
