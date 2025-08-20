
#include "ShopView.hh"
#include "ComputerListMessage.hh"
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

ShopView::ShopView(const bsgo::Repositories &repositories)
  : AbstractView("shop")
  , m_repositories(repositories)
{}

void ShopView::setPlayerDbId(const bsgo::Uuid player)
{
  m_playerDbId = player;
}

bool ShopView::isReady() const noexcept
{
  return m_playerDbId.has_value() && !m_resources.empty() && !m_computers.empty()
         && !m_weapons.empty();
}

void ShopView::onMessageReceived(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::RESOURCE_LIST:
      m_resources = message.as<bsgo::ResourceListMessage>().getResourcesData();
      break;
    case bsgo::MessageType::COMPUTER_LIST:
      m_computers = message.as<bsgo::ComputerListMessage>().getComputersData();
      break;
    case bsgo::MessageType::WEAPON_LIST:
      m_weapons = message.as<bsgo::WeaponListMessage>().getWeaponsData();
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
  }
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
  checkPlayerDbIdExists();

  bsgo::AffordabilityData data{
    .playerId          = *m_playerDbId,
    .itemId            = id,
    .itemType          = itemType,
    .resourceRepo      = m_repositories.playerResourceRepository,
    .weaponPriceRepo   = m_repositories.weaponPriceRepository,
    .computerPriceRepo = m_repositories.computerPriceRepository,
    .shipPriceRepo     = m_repositories.shipPriceRepository,
  };
  return computeAffordability(data);
}

auto ShopView::getAllShipsForFaction(const bsgo::Faction &faction) const -> std::vector<bsgo::Ship>
{
  return m_repositories.shipRepository->findAllByFaction(faction);
}

void ShopView::checkPlayerDbIdExists() const
{
  if (!m_playerDbId)
  {
    error("Expected player db id to exist but it does not");
  }
}

} // namespace pge
