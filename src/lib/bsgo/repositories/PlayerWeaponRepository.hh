
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

class PlayerWeaponRepository : public IRepository
{
  public:
  PlayerWeaponRepository();
  ~PlayerWeaponRepository() override = default;

  auto findAllById(const Uuid &player) const -> std::vector<Uuid>;
};

using PlayerWeaponRepositoryShPtr = std::shared_ptr<PlayerWeaponRepository>;

} // namespace bsgo
