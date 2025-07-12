
#pragma once

#include "AbstractView.hh"
#include "Faction.hh"
#include "PurchaseUtils.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_map>
#include <vector>

namespace pge {

struct ResourceCost
{
  bsgo::Resource resource{};
  float amount{};
};

struct ShopItem
{
  std::vector<ResourceCost> price;
  std::optional<bsgo::Weapon> weapon{};
  std::optional<bsgo::Computer> computer{};

  auto id() const -> bsgo::Uuid;
  auto type() const -> bsgo::Item;
};

class ShopView : public AbstractView
{
  public:
  ShopView(const bsgo::Repositories &repositories);
  ~ShopView() override = default;

  void setPlayerDbId(const bsgo::Uuid player);
  bool isReady() const noexcept override;

  auto getShopItems() const -> std::vector<ShopItem>;
  auto canPlayerAfford(const bsgo::Uuid id, const bsgo::Item &itemType) const
    -> bsgo::Affordability;

  auto getAllShipsForFaction(const bsgo::Faction &faction) const -> std::vector<bsgo::Ship>;

  private:
  bsgo::Repositories m_repositories{};
  std::optional<bsgo::Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;

  auto getWeaponAsShopItem(const bsgo::Uuid weaponId) const -> ShopItem;
  auto getComputerAsShopItem(const bsgo::Uuid computerId) const -> ShopItem;
};

using ShopViewShPtr = std::shared_ptr<ShopView>;

} // namespace pge
