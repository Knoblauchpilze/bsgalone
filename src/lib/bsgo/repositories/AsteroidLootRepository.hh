
#pragma once

#include "IRepository.hh"
#include "Item.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

struct AsteroidLoot
{
  Uuid resource;
  float amount;
};

class AsteroidLootRepository : public IRepository
{
  public:
  AsteroidLootRepository();
  ~AsteroidLootRepository() override = default;

  auto findOneById(const Uuid &asteroid) const -> AsteroidLoot;
};

using AsteroidLootRepositoryShPtr = std::shared_ptr<AsteroidLootRepository>;

} // namespace bsgo
