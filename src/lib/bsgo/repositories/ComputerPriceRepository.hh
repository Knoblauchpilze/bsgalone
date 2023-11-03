
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_map>

namespace bsgo {

class ComputerPriceRepository : public IRepository
{
  public:
  ComputerPriceRepository();
  ~ComputerPriceRepository() override = default;

  auto findAllByComputer(const Uuid &computer) const -> std::unordered_map<Uuid, int>;
};

using ComputerPriceRepositoryShPtr = std::shared_ptr<ComputerPriceRepository>;

} // namespace bsgo
