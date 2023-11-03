
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_map>

namespace bsgo {

class WeaponPriceRepository : public IRepository
{
  public:
  WeaponPriceRepository();
  ~WeaponPriceRepository() override = default;

  auto findAllByWeapon(const Uuid &weapon) const -> std::unordered_map<Uuid, int>;
};

using WeaponPriceRepositoryShPtr = std::shared_ptr<WeaponPriceRepository>;

} // namespace bsgo
