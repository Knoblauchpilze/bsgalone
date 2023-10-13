
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgo {

struct SystemShip
{
  Uuid model{};
  Uuid ship{};
};

class SystemRepository : public IRepository
{
  public:
  SystemRepository();
  ~SystemRepository() override = default;

  auto findAllAsteroidsBySystem(const Uuid &system) const -> std::vector<Uuid>;
  auto findAllShipsBySystem(const Uuid &system) const -> std::vector<SystemShip>;
  auto findAllOutpostsBySystem(const Uuid &system) const -> std::vector<Uuid>;
};

using SystemRepositoryShPtr = std::shared_ptr<SystemRepository>;

} // namespace bsgo
