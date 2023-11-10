
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_set>

namespace bsgo {

class SystemRepository : public AbstractRepository
{
  public:
  SystemRepository(const DbConnectionShPtr &connection);
  ~SystemRepository() override = default;

  auto findAllAsteroidsBySystem(const Uuid &system) const -> std::unordered_set<Uuid>;
  auto findAllShipsBySystem(const Uuid &system) const -> std::unordered_set<Uuid>;
  auto findAllOutpostsBySystem(const Uuid &system) const -> std::unordered_set<Uuid>;
};

using SystemRepositoryShPtr = std::shared_ptr<SystemRepository>;

} // namespace bsgo
