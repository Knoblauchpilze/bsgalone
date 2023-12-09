
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_map>

namespace bsgo {

class WeaponPriceRepository : public AbstractRepository
{
  public:
  WeaponPriceRepository(const DbConnectionShPtr &connection);
  ~WeaponPriceRepository() override = default;

  void initialize() override;

  auto findAllByWeapon(const Uuid &weapon) const -> std::unordered_map<Uuid, int>;
};

using WeaponPriceRepositoryShPtr = std::shared_ptr<WeaponPriceRepository>;

} // namespace bsgo
