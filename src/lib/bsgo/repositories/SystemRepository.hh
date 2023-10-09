
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

  auto findAsteroids(const Uuid &system) const -> std::vector<Uuid>;
  auto findShips(const Uuid &system) const -> std::vector<SystemShip>;
};

using SystemRepositoryShPtr = std::shared_ptr<SystemRepository>;

} // namespace bsgo
