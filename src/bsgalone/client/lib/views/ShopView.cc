
#include "ShopView.hh"
#include "ComputerListMessage.hh"
#include "PlayerResourceListMessage.hh"
#include "ResourceListMessage.hh"
#include "WeaponListMessage.hh"

namespace bsgalone::client {

auto ShopItem::id() const -> core::Uuid
{
  if (weapon)
  {
    return weapon->dbId;
  }
  else if (computer)
  {
    return computer->dbId;
  }

  throw std::invalid_argument("Expected shop item to be a weapon or a computer");
}

auto ShopItem::type() const -> core::Item
{
  if (weapon)
  {
    return core::Item::WEAPON;
  }
  else if (computer)
  {
    return core::Item::COMPUTER;
  }

  throw std::invalid_argument("Expected shop item to be a weapon or a computer");
}

ShopView::ShopView(GameSessionShPtr gameSession)
  : AbstractView("shop",
                 {
                   core::MessageType::COMPUTER_LIST,
                   core::MessageType::PLAYER_RESOURCE_LIST,
                   core::MessageType::RESOURCE_LIST,
                   core::MessageType::SHIP_LIST,
                   core::MessageType::WEAPON_LIST,
                 })
  , m_gameSession(std::move(gameSession))
{
  if (nullptr == m_gameSession)
  {
    throw std::invalid_argument("Expected non null game session");
  }
}

bool ShopView::isReady() const noexcept
{
  return m_gameSession->hasPlayerDbId() && m_gameSession->hasTimeStep() && !m_resources.empty()
         && !m_computers.empty() && !m_weapons.empty() && !m_ships.empty()
         && !m_playerResources.empty();
}

void ShopView::reset()
{
  m_resources.clear();
  m_computers.clear();
  m_weapons.clear();
  m_ships.clear();
  m_playerResources.clear();
}

auto ShopView::gameSession() const -> const GameSession &
{
  return *m_gameSession;
}

namespace {
auto tryFindResource(const core::Uuid resourceId, const std::vector<core::ResourceData> &resources)
  -> std::optional<core::ResourceData>
{
  const auto maybeResource = std::find_if(resources.begin(),
                                          resources.end(),
                                          [&resourceId](const core::ResourceData &resource) {
                                            return resource.dbId == resourceId;
                                          });

  if (maybeResource == resources.end())
  {
    return {};
  }

  return *maybeResource;
}

auto getWeaponAsShopItem(const core::WeaponData &weapon,
                         const std::vector<core::ResourceData> &resources) -> ShopItem
{
  ShopItem out;
  out.weapon = weapon;

  for (const auto &[resourceId, cost] : weapon.price)
  {
    const auto maybeResource = tryFindResource(resourceId, resources);
    if (!maybeResource)
    {
      throw std::invalid_argument("Unsupported resource " + core::str(resourceId));
    }

    out.price.emplace_back(*maybeResource, cost);
  }

  return out;
}

auto getComputerAsShopItem(const core::ComputerData &computer,
                           const std::vector<core::ResourceData> &resources) -> ShopItem
{
  ShopItem out;
  out.computer = computer;

  for (const auto &[resourceId, cost] : computer.price)
  {
    const auto maybeResource = tryFindResource(resourceId, resources);
    if (!maybeResource)
    {
      throw std::invalid_argument("Unsupported resource " + core::str(resourceId));
    }

    out.price.emplace_back(*maybeResource, cost);
  }

  return out;
}
} // namespace

auto ShopView::getShopItems() const -> std::vector<ShopItem>
{
  std::vector<ShopItem> out;

  for (const auto &weapon : m_weapons)
  {
    out.push_back(getWeaponAsShopItem(weapon, m_resources));
  }

  for (const auto &computer : m_computers)
  {
    out.push_back(getComputerAsShopItem(computer, m_resources));
  }

  return out;
}

auto ShopView::canPlayerAfford(const core::Uuid id, const core::Item &itemType) const
  -> core::Affordability
{
  core::AffordabilityData data{
    .playerId = m_gameSession->getPlayerDbId(),
    .itemId   = id,
    .itemType = itemType,

    .playerResources = m_playerResources,
    .weapons         = m_weapons,
    .computers       = m_computers,
    .ships           = m_ships,
  };
  return computeAffordability(data);
}

auto ShopView::getAllShips() const -> std::vector<core::ShipData>
{
  return m_ships;
}

void ShopView::handleMessageInternal(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::RESOURCE_LIST:
      m_resources = message.as<core::ResourceListMessage>().getResourcesData();
      break;
    case core::MessageType::COMPUTER_LIST:
      m_computers = message.as<core::ComputerListMessage>().getComputersData();
      break;
    case core::MessageType::WEAPON_LIST:
      m_weapons = message.as<core::WeaponListMessage>().getWeaponsData();
      break;
    case core::MessageType::SHIP_LIST:
      handleShipsLoading(message.as<core::ShipListMessage>());
      break;
    case core::MessageType::PLAYER_RESOURCE_LIST:
      m_playerResources = message.as<core::PlayerResourceListMessage>().getResourcesData();
      break;
    default:
      error("Unsupported message type " + str(message.type()));
  }
}

void ShopView::handleShipsLoading(const core::ShipListMessage &message)
{
  if (message.getFaction() != m_gameSession->getFaction())
  {
    error("Received ships for wrong faction, expected " + core::str(m_gameSession->getFaction())
          + ", got " + core::str(message.getFaction()));
  }

  m_ships = message.getShipsData();
}

} // namespace bsgalone::client
