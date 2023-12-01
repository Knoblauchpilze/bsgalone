
#pragma once

#include "IView.hh"
#include <memory>

namespace bsgo {

struct ResourceCost
{
  Resource resource;
  int amount;
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

  auto getShopItems() const -> std::vector<ShopItem>;

  private:
  void getWeaponItems(std::vector<ShopItem> &items) const;
  void getComputerItems(std::vector<ShopItem> &items) const;
};

using ShopViewShPtr = std::shared_ptr<ShopView>;

} // namespace bsgo
