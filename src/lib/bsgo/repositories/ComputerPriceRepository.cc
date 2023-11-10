
#include "ComputerPriceRepository.hh"

namespace bsgo {

ComputerPriceRepository::ComputerPriceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("computer", connection)
{
  addModule("price");
}

auto ComputerPriceRepository::findAllByComputer(const Uuid &computer) const
  -> std::unordered_map<Uuid, int>
{
  switch (computer)
  {
    case Uuid{0}:
      return {{Uuid{0}, 1}, {Uuid{1}, 6}};
    default:
      error("Computer " + str(computer) + " not found");
      break;
  }

  // Can't happen because of the error above.
  return {};
}

} // namespace bsgo
