
#include "ShopView.hh"
#include "ComputerListMessage.hh"
#include "PlayerResourceListMessage.hh"
#include "ResourceListMessage.hh"
#include "WeaponListMessage.hh"

namespace pge {

auto ShopItem::id() const -> bsgo::Uuid
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

auto ShopItem::type() const -> bsgo::Item
{
  if (weapon)
  {
    return bsgo::Item::WEAPON;
  }
  else if (computer)
  {
    return bsgo::Item::COMPUTER;
  }

  throw std::invalid_argument("Expected shop item to be a weapon or a computer");
}

ShopView::ShopView(GameSessionShPtr gameSession)
  : AbstractView("shop",
                 {
                   bsgalone::core::MessageType::COMPUTER_LIST,
                   bsgalone::core::MessageType::PLAYER_RESOURCE_LIST,
                   bsgalone::core::MessageType::RESOURCE_LIST,
                   bsgalone::core::MessageType::SHIP_LIST,
                   bsgalone::core::MessageType::WEAPON_LIST,
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
auto tryFindResource(const bsgo::Uuid resourceId, const std::vector<bsgo::ResourceData> &resources)
  -> std::optional<bsgo::ResourceData>
{
  const auto maybeResource = std::find_if(resources.begin(),
                                          resources.end(),
                                          [&resourceId](const bsgo::ResourceData &resource) {
                                            return resource.dbId == resourceId;
                                          });

  if (maybeResource == resources.end())
  {
    return {};
  }

  return *maybeResource;
}

auto getWeaponAsShopItem(const bsgo::WeaponData &weapon,
                         const std::vector<bsgo::ResourceData> &resources) -> ShopItem
{
  ShopItem out;
  out.weapon = weapon;

  for (const auto &[resourceId, cost] : weapon.price)
  {
    const auto maybeResource = tryFindResource(resourceId, resources);
    if (!maybeResource)
    {
      throw std::invalid_argument("Unsupported resource " + bsgo::str(resourceId));
    }

    out.price.emplace_back(*maybeResource, cost);
  }

  return out;
}

auto getComputerAsShopItem(const bsgo::ComputerData &computer,
                           const std::vector<bsgo::ResourceData> &resources) -> ShopItem
{
  ShopItem out;
  out.computer = computer;

  for (const auto &[resourceId, cost] : computer.price)
  {
    const auto maybeResource = tryFindResource(resourceId, resources);
    if (!maybeResource)
    {
      throw std::invalid_argument("Unsupported resource " + bsgo::str(resourceId));
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

auto ShopView::canPlayerAfford(const bsgo::Uuid id, const bsgo::Item &itemType) const
  -> bsgo::Affordability
{
  bsgo::AffordabilityData data{
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

auto ShopView::getAllShips() const -> std::vector<bsgo::ShipData>
{
  return m_ships;
}

void ShopView::handleMessageInternal(const bsgalone::core::IMessage &message)
{
  switch (message.type())
  {
    case bsgalone::core::MessageType::RESOURCE_LIST:
      m_resources = message.as<bsgo::ResourceListMessage>().getResourcesData();
      break;
    case bsgalone::core::MessageType::COMPUTER_LIST:
      m_computers = message.as<bsgo::ComputerListMessage>().getComputersData();
      break;
    case bsgalone::core::MessageType::WEAPON_LIST:
      m_weapons = message.as<bsgo::WeaponListMessage>().getWeaponsData();
      break;
    case bsgalone::core::MessageType::SHIP_LIST:
      handleShipsLoading(message.as<bsgo::ShipListMessage>());
      break;
    case bsgalone::core::MessageType::PLAYER_RESOURCE_LIST:
      m_playerResources = message.as<bsgo::PlayerResourceListMessage>().getResourcesData();
      break;
    default:
      error("Unsupported message type " + str(message.type()));
  }
}

void ShopView::handleShipsLoading(const bsgo::ShipListMessage &message)
{
  if (message.getFaction() != m_gameSession->getFaction())
  {
    error("Received ships for wrong faction, expected " + bsgo::str(m_gameSession->getFaction())
          + ", got " + bsgo::str(message.getFaction()));
  }

  m_ships = message.getShipsData();
}

} // namespace pge
