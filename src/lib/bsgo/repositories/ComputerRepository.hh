
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <optional>

namespace bsgo {

struct Computer
{
  bool offensive;
  float powerCost;
  float range;
  utils::Duration reloadTime;

  std::optional<utils::Duration> duration{};
  std::optional<float> damageModifier{};
};

class ComputerRepository : public IRepository
{
  public:
  ComputerRepository();
  ~ComputerRepository() override = default;

  auto findOneById(const Uuid &computer) const -> Computer;
};

} // namespace bsgo
