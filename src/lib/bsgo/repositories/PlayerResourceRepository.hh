
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgo {

struct Resource
{
  Uuid resource;
  float amount;
};

class PlayerResourceRepository : public IRepository
{
  public:
  PlayerResourceRepository();
  ~PlayerResourceRepository() override = default;

  auto findOneById(const Uuid &player) const -> std::vector<Resource>;
  auto findOneByIdAndResource(const Uuid &player, const Uuid &resource) const -> Resource;

  void save(Resource resource);
};

using PlayerResourceRepositoryShPtr = std::shared_ptr<PlayerResourceRepository>;

} // namespace bsgo
