
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_set>

namespace bsgo {

class SystemRepository : public IRepository
{
  public:
  SystemRepository();
  ~SystemRepository() override = default;

  auto findAllAsteroidsBySystem(const Uuid &system) const -> std::unordered_set<Uuid>;
  auto findAllShipsBySystem(const Uuid &system) const -> std::unordered_set<Uuid>;
  auto findAllOutpostsBySystem(const Uuid &system) const -> std::unordered_set<Uuid>;
};

using SystemRepositoryShPtr = std::shared_ptr<SystemRepository>;

} // namespace bsgo
