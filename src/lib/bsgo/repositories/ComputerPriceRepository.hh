
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_map>

namespace bsgo {

class ComputerPriceRepository : public AbstractRepository
{
  public:
  ComputerPriceRepository(const DbConnectionShPtr &connection);
  ~ComputerPriceRepository() override = default;

  auto findAllByComputer(const Uuid &computer) const -> std::unordered_map<Uuid, int>;
};

using ComputerPriceRepositoryShPtr = std::shared_ptr<ComputerPriceRepository>;

} // namespace bsgo
