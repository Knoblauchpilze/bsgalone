
#pragma once

#include "AbstractView.hh"
#include "ComputerData.hh"
#include "GameSession.hh"
#include "PlayerResourceData.hh"
#include "PurchaseUtils.hh"
#include "ResourceData.hh"
#include "ShipData.hh"
#include "ShipListMessage.hh"
#include "Uuid.hh"
#include "WeaponData.hh"
#include <memory>
#include <vector>

namespace pge {

struct ResourceCost
{
  bsgo::ResourceData resource{};
  int amount{};
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
  ShopView(GameSessionShPtr gameSession);
  ~ShopView() override = default;

  bool isReady() const noexcept override;
  void reset() override;

  auto getShopItems() const -> std::vector<ShopItem>;
  auto canPlayerAfford(const bsgo::Uuid id, const bsgo::Item &itemType) const
    -> bsgo::Affordability;

  auto getAllShips() const -> std::vector<bsgo::ShipData>;

  protected:
  void handleMessageInternal(const bsgo::IMessage &message) override;

  private:
  GameSessionShPtr m_gameSession{};
  std::vector<bsgo::ResourceData> m_resources{};
  std::vector<bsgo::ComputerData> m_computers{};
  std::vector<bsgo::WeaponData> m_weapons{};
  std::vector<bsgo::ShipData> m_ships{};
  std::vector<bsgo::PlayerResourceData> m_playerResources{};

  void handleShipsLoading(const bsgo::ShipListMessage &message);
};

using ShopViewShPtr = std::shared_ptr<ShopView>;

} // namespace pge
