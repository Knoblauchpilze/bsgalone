
#pragma once

#include "IView.hh"
#include "PurchaseUtils.hh"
#include <memory>
#include <unordered_map>

namespace bsgo {

struct ResourceCost
{
  Resource resource{};
  float amount{};
};

struct ShopItem
{
  std::vector<ResourceCost> price;
  std::optional<Weapon> weapon{};
  std::optional<Computer> computer{};

  auto id() const -> Uuid;
  auto type() const -> Item;
};

class ShopView : public IView
{
  public:
  ShopView(const CoordinatorShPtr &coordinator, const Repositories &repositories);
  ~ShopView() override = default;

  void setPlayerDbId(const Uuid &player);
  bool isReady() const noexcept override;

  auto getShopItems() const -> std::vector<ShopItem>;
  auto canPlayerAfford(const Uuid &id, const Item &type) const -> Affordability;

  private:
  std::optional<Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;

  auto getWeaponAsShopItem(const Uuid &weaponId) const -> ShopItem;
  auto getComputerAsShopItem(const Uuid &computerId) const -> ShopItem;
};

using ShopViewShPtr = std::shared_ptr<ShopView>;

} // namespace bsgo
