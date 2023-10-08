
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgo {

class SystemRepository : public IRepository
{
  public:
  SystemRepository();
  ~SystemRepository() override = default;

  auto findAsteroids(const Uuid &system) -> std::vector<Uuid>;
  auto findShips(const Uuid &system) -> std::vector<Uuid>;
};

using SystemRepositoryShPtr = std::shared_ptr<SystemRepository>;

} // namespace bsgo
