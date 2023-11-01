
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgo {

struct Resource
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

  auto findAllByPlayer(const Uuid &player) const -> std::vector<Resource>;
  auto findOneByIdAndResource(const Uuid &player, const Uuid &resource) const -> Resource;

  void save(Resource resource);
};

using PlayerResourceRepositoryShPtr = std::shared_ptr<PlayerResourceRepository>;

} // namespace bsgo
