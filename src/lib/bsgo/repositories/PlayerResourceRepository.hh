
#pragma once

#include "IRepository.hh"
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

class PlayerResourceRepository : public IRepository
{
  public:
  PlayerResourceRepository();
  ~PlayerResourceRepository() override = default;

  auto findOneById(const Uuid &resource) const -> PlayerResource;
  auto findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>;
};

using PlayerResourceRepositoryShPtr = std::shared_ptr<PlayerResourceRepository>;

} // namespace bsgo
