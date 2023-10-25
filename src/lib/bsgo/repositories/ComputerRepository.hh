
#pragma once

#include "EntityKind.hh"
#include "IRepository.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <optional>
#include <unordered_set>

namespace bsgo {

struct Computer
{
  bool offensive;
  float powerCost;
  std::optional<float> range{};
  utils::Duration reloadTime;

  std::optional<utils::Duration> duration{};
  std::optional<std::unordered_set<EntityKind>> allowedTargets{};
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
