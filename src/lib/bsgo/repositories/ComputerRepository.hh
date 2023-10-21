
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>

namespace bsgo {

struct Computer
{
  float powerCost;

  float range;

  utils::Duration reloadTime;
};

class ComputerRepository : public IRepository
{
  public:
  ComputerRepository();
  ~ComputerRepository() override = default;

  auto findOneById(const Uuid &computer) const -> Computer;
};

} // namespace bsgo
