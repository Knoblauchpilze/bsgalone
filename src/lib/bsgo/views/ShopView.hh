
#pragma once

#include "AbstractView.hh"
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

class ShopView : public AbstractView
{
  public:
  ShopView(const CoordinatorShPtr &coordinator,
           const Repositories &repositories,
           const IMessageQueue *messageQueue);
  ~ShopView() override = default;

  void setPlayerDbId(const Uuid &player);
  bool isReady() const noexcept override;

  auto getShopItems() const -> std::vector<ShopItem>;
  auto canPlayerAfford(const Uuid &id, const Item &type) const -> Affordability;

  auto getAllShipsForFaction(const Faction &faction) const -> std::vector<Ship>;

  private:
  std::optional<Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;

  auto getWeaponAsShopItem(const Uuid &weaponId) const -> ShopItem;
  auto getComputerAsShopItem(const Uuid &computerId) const -> ShopItem;
};

using ShopViewShPtr = std::shared_ptr<ShopView>;

} // namespace bsgo
