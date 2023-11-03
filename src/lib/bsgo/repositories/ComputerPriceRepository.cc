
#include "ComputerPriceRepository.hh"

namespace bsgo {

ComputerPriceRepository::ComputerPriceRepository()
  : IRepository("computer")
{
  addModule("price");
}

auto ComputerPriceRepository::findAllByComputer(const Uuid &computer) const
  -> std::unordered_map<Uuid, int>
{
  error("Computer " + str(computer) + " not found");
  return {};
}

} // namespace bsgo
