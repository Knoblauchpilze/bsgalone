
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
  Uuid id{};
  Uuid computer{};
  Uuid player{};

  std::string name{};
  int level{};

  bool offensive{};
  float powerCost{};
  std::optional<float> range{};
  utils::Duration reloadTime{};

  std::optional<utils::Duration> duration{};
  std::optional<std::unordered_set<EntityKind>> allowedTargets{};
  std::optional<float> damageModifier{};
};

class PlayerComputerRepository : public AbstractRepository
{
  public:
  PlayerComputerRepository(const DbConnectionShPtr &connection);
  ~PlayerComputerRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid computer) const -> PlayerComputer;
  auto findAllByPlayer(const Uuid player) const -> std::unordered_set<Uuid>;

  void save(const PlayerComputer &computer);

  private:
  auto fetchComputerBase(const Uuid computer) const -> PlayerComputer;
  void fetchAllowedTargets(const Uuid computer, PlayerComputer &out) const;
};

using PlayerComputerRepositoryShPtr = std::shared_ptr<PlayerComputerRepository>;

} // namespace bsgo
