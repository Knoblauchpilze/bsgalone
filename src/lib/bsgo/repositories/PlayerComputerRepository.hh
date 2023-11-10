
#pragma once

#include "AbstractRepository.hh"
#include "EntityKind.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgo {

struct PlayerComputer
{
  std::string name;
  int level;

  bool offensive;
  float powerCost;
  std::optional<float> range{};
  utils::Duration reloadTime;

  std::optional<utils::Duration> duration{};
  std::optional<std::unordered_set<EntityKind>> allowedTargets{};
  std::optional<float> damageModifier{};
};

class PlayerComputerRepository : public AbstractRepository
{
  public:
  PlayerComputerRepository(const DbConnectionShPtr &connection);
  ~PlayerComputerRepository() override = default;

  auto findOneById(const Uuid &computer) const -> PlayerComputer;
  auto findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>;
};

using PlayerComputerRepositoryShPtr = std::shared_ptr<PlayerComputerRepository>;

} // namespace bsgo
