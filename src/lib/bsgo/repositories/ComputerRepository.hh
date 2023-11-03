
#pragma once

#include "EntityKind.hh"
#include "IRepository.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgo {

struct Computer
{
  std::string name;
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

  auto findAll() const -> std::unordered_set<Uuid>;
  auto findOneById(const Uuid &computer) const -> Computer;
};

using ComputerRepositoryShPtr = std::shared_ptr<ComputerRepository>;

} // namespace bsgo
