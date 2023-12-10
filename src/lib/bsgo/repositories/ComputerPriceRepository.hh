
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

  void initialize() override;

  auto findAllByComputer(const Uuid &computer) const -> std::unordered_map<Uuid, float>;
};

using ComputerPriceRepositoryShPtr = std::shared_ptr<ComputerPriceRepository>;

} // namespace bsgo
