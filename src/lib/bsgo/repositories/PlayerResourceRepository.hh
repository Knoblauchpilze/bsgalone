
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_set>

namespace bsgo {

struct PlayerResource
{
  Uuid resource;
  std::string name;
  float amount;
};

class PlayerResourceRepository : public AbstractRepository
{
  public:
  PlayerResourceRepository(const DbConnectionShPtr &connection);
  ~PlayerResourceRepository() override = default;

  auto findAllByPlayer(const Uuid &player) const -> std::vector<PlayerResource>;
};

using PlayerResourceRepositoryShPtr = std::shared_ptr<PlayerResourceRepository>;

} // namespace bsgo
