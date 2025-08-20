
#pragma once

#include "AbstractView.hh"
#include "ComputerData.hh"
#include "Faction.hh"
#include "PurchaseUtils.hh"
#include "Repositories.hh"
#include "ResourceData.hh"
#include "Uuid.hh"
#include "WeaponData.hh"
#include <memory>
#include <vector>

namespace pge {

struct ResourceCost
{
  bsgo::ResourceData resource{};
  float amount{};
};

struct ShopItem
{
  std::vector<ResourceCost> price;
  std::optional<bsgo::WeaponData> weapon{};
  std::optional<bsgo::ComputerData> computer{};

  auto id() const -> bsgo::Uuid;
  auto type() const -> bsgo::Item;
};

class ShopView : public AbstractView
{
  public:
  ShopView(const bsgo::Repositories &repositories);
  ~ShopView() override = default;

  // TODO: should be removed
  void setPlayerDbId(const bsgo::Uuid player);
  bool isReady() const noexcept override;

  void onMessageReceived(const bsgo::IMessage &message) override;

  auto getShopItems() const -> std::vector<ShopItem>;
  auto canPlayerAfford(const bsgo::Uuid id, const bsgo::Item &itemType) const
    -> bsgo::Affordability;

  auto getAllShipsForFaction(const bsgo::Faction &faction) const -> std::vector<bsgo::Ship>;

  private:
  bsgo::Repositories m_repositories{};
  std::vector<bsgo::ResourceData> m_resources{};
  std::vector<bsgo::ComputerData> m_computers{};
  std::vector<bsgo::WeaponData> m_weapons{};
  std::optional<bsgo::Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;
};

using ShopViewShPtr = std::shared_ptr<ShopView>;

} // namespace pge
