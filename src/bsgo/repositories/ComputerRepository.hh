
#pragma once

#include "AbstractRepository.hh"
#include "EntityKind.hh"
#include "TickDuration.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgo {

struct Computer
{
  Uuid id{};
  std::string name{};
  bool offensive{};
  float powerCost{};
  std::optional<float> range{};
  chrono::TickDuration reloadTime{};

  std::optional<chrono::TickDuration> duration{};
  std::optional<std::unordered_set<EntityKind>> allowedTargets{};
  std::optional<float> damageModifier{};
};

class ComputerRepository : public AbstractRepository
{
  public:
  ComputerRepository(const DbConnectionShPtr &connection);
  ~ComputerRepository() override = default;

  void initialize() override;

  auto findAll() const -> std::unordered_set<Uuid>;
  auto findOneById(const Uuid computer) const -> Computer;

  private:
  auto fetchComputerBase(const Uuid computer) const -> Computer;
  void fetchAllowedTargets(const Uuid computer, Computer &out) const;
};

using ComputerRepositoryShPtr = std::shared_ptr<ComputerRepository>;

} // namespace bsgo
