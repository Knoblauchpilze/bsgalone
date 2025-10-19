
#pragma once

#include "AbstractRepository.hh"
#include "Tick.hh"
#include "TimeStep.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

struct SystemTick
{
  Uuid system{};

  chrono::Tick currentTick{};
  chrono::TimeStep step{};
};

class TickRepository : public AbstractRepository
{
  public:
  TickRepository(const DbConnectionShPtr &connection);
  ~TickRepository() override = default;

  void initialize() override;

  auto findOneBySystem(const Uuid system) const -> SystemTick;

  void save(const SystemTick &tickData);
};

using TickRepositoryShPtr = std::shared_ptr<TickRepository>;

} // namespace bsgo
