
#include "ShopView.hh"
#include <stdexcept>

namespace bsgo {

auto ShopItem::id() const -> Uuid
{
  if (weapon)
  {
    return weapon->id;
  }
  else if (computer)
  {
    return computer->id;
  }

  throw std::invalid_argument("Expected shop item to be a weapon or a computer");
}

auto ShopItem::type() const -> Item
{
  if (weapon)
  {
    return Item::WEAPON;
  }
  else if (computer)
  {
    return Item::COMPUTER;
  }

  throw std::invalid_argument("Expected shop item to be a weapon or a computer");
}

ShopView::ShopView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : AbstractView("shop", coordinator, repositories)
{}

void ShopView::setPlayerDbId(const Uuid &player)
{
  m_playerDbId = player;
}

bool ShopView::isReady() const noexcept
{
  return m_playerDbId.has_value();
}

auto ShopView::getShopItems() const -> std::vector<ShopItem>
{
  std::vector<ShopItem> out;

  const auto weapons = m_repositories.weaponRepository->findAll();
  for (const auto &weaponId : weapons)
  {
    out.push_back(getWeaponAsShopItem(weaponId));
  }

  const auto computers = m_repositories.computerRepository->findAll();
  for (const auto &computerId : computers)
  {
    out.push_back(getComputerAsShopItem(computerId));
  }

  return out;
}

auto ShopView::canPlayerAfford(const Uuid &id, const Item &type) const -> Affordability
{
  checkPlayerDbIdExists();

  AffordabilityData data{
    .playerId          = *m_playerDbId,
    .itemId            = id,
    .itemType          = type,
    .resourceRepo      = m_repositories.playerResourceRepository,
    .weaponPriceRepo   = m_repositories.weaponPriceRepository,
    .computerPriceRepo = m_repositories.computerPriceRepository,
    .shipPriceRepo     = m_repositories.shipPriceRepository,
  };
  return computeAffordability(data);
}

auto ShopView::getAllShipsForFaction(const Faction &faction) const -> std::vector<Ship>
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

auto ShopView::getWeaponAsShopItem(const Uuid &weaponId) const -> ShopItem
{
  ShopItem item{};
  item.weapon = m_repositories.weaponRepository->findOneById(weaponId);

  const auto price = m_repositories.weaponPriceRepository->findAllByWeapon(weaponId);
  for (const auto &[resourceId, cost] : price)
  {
    const auto resource = m_repositories.resourceRepository->findOneById(resourceId);
    item.price.push_back(ResourceCost{.resource = resource, .amount = cost});
  }

  return item;
}

auto ShopView::getComputerAsShopItem(const Uuid &computerId) const -> ShopItem
{
  ShopItem item{};
  item.computer = m_repositories.computerRepository->findOneById(computerId);

  const auto price = m_repositories.computerPriceRepository->findAllByComputer(computerId);
  for (const auto &[resourceId, cost] : price)
  {
    const auto resource = m_repositories.resourceRepository->findOneById(resourceId);
    item.price.push_back(ResourceCost{.resource = resource, .amount = cost});
  }

  return item;
}

} // namespace bsgo
