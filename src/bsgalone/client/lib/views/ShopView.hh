
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

namespace bsgalone::client {

struct ResourceCost
{
  core::ResourceData resource{};
  int amount{};
};

struct ShopItem
{
  std::vector<ResourceCost> price;
  std::optional<core::WeaponData> weapon{};
  std::optional<core::ComputerData> computer{};

  auto id() const -> core::Uuid;
  auto type() const -> core::Item;
};

class ShopView : public AbstractView
{
  public:
  ShopView(GameSessionShPtr gameSession);
  ~ShopView() override = default;

  bool isReady() const noexcept override;
  void reset() override;

  auto gameSession() const -> const GameSession &;

  auto getShopItems() const -> std::vector<ShopItem>;
  auto canPlayerAfford(const core::Uuid id, const core::Item &itemType) const
    -> core::Affordability;

  auto getAllShips() const -> std::vector<core::ShipData>;

  protected:
  void handleMessageInternal(const core::IMessage &message) override;

  private:
  GameSessionShPtr m_gameSession{};
  std::vector<core::ResourceData> m_resources{};
  std::vector<core::ComputerData> m_computers{};
  std::vector<core::WeaponData> m_weapons{};
  std::vector<core::ShipData> m_ships{};
  std::vector<core::PlayerResourceData> m_playerResources{};

  void handleShipsLoading(const core::ShipListMessage &message);
};

using ShopViewShPtr = std::shared_ptr<ShopView>;

} // namespace bsgalone::client
