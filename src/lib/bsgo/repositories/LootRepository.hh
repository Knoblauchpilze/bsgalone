
#pragma once

#include "IRepository.hh"
#include "Item.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

struct Loot
{
  Item type;
  Uuid id;
};

class LootRepository : public IRepository
{
  public:
  LootRepository();
  ~LootRepository() override = default;

  auto findOneById(const Uuid &loot) const -> Loot;
};

using LootRepositoryShPtr = std::shared_ptr<LootRepository>;

} // namespace bsgo
