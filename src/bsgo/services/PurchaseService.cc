
#include "PurchaseService.hh"
#include "PurchaseUtils.hh"

namespace bsgo {

PurchaseService::PurchaseService(const Repositories &repositories)
  : AbstractService("purchase", repositories)
{}

bool PurchaseService::tryPurchase(const Uuid playerId, const Uuid itemId, const Item &type) const
{
  if (!verifyAffordability(playerId, itemId, type))
  {
    return false;
  }
  if (!verifyPreconditions(playerId, itemId, type))
  {
    return false;
  }

  switch (type)
  {
    case Item::WEAPON:
      tryPurchaseWeapon(playerId, itemId);
      break;
    case Item::COMPUTER:
      tryPurchaseComputer(playerId, itemId);
      break;
    case Item::SHIP:
      tryPurchaseShip(playerId, itemId);
      break;
    default:
      error("Invalid kind of item to buy", "Unsupported item " + str(type));
      break;
  }

  return true;
}

bool PurchaseService::verifyAffordability(const Uuid playerId,
                                          const Uuid itemId,
                                          const Item &type) const
{
  AffordabilityData data{
    .playerId          = playerId,
    .itemId            = itemId,
    .itemType          = type,
    .resourceRepo      = m_repositories.playerResourceRepository,
    .weaponPriceRepo   = m_repositories.weaponPriceRepository,
    .computerPriceRepo = m_repositories.computerPriceRepository,
    .shipPriceRepo     = m_repositories.shipPriceRepository,
  };

  const auto affordability = computeAffordability(data);
  return affordability.canAfford;
}

bool PurchaseService::verifyPreconditions(const Uuid playerId,
                                          const Uuid itemId,
                                          const Item &type) const
{
  if (Item::SHIP == type)
  {
    const auto shipsIds = m_repositories.playerShipRepository->findAllByPlayer(playerId);
    for (const auto &shipId : shipsIds)
    {
      const auto ship = m_repositories.playerShipRepository->findOneById(shipId);
      if (itemId == ship.ship)
      {
        return false;
      }
    }
  }

  return true;
}

void PurchaseService::tryPurchaseWeapon(const Uuid playerId, const Uuid weaponId) const
{
  const auto costs = m_repositories.weaponPriceRepository->findAllByWeapon(weaponId);
  debitResources(playerId, costs);

  PlayerWeapon weapon{
    .weapon = weaponId,
    .player = playerId,
  };
  m_repositories.playerWeaponRepository->save(weapon);
}

void PurchaseService::tryPurchaseComputer(const Uuid playerId, const Uuid computerId) const
{
  const auto costs = m_repositories.computerPriceRepository->findAllByComputer(computerId);
  debitResources(playerId, costs);

  PlayerComputer computer{
    .computer = computerId,
    .player   = playerId,
  };
  m_repositories.playerComputerRepository->save(computer);
}

void PurchaseService::tryPurchaseShip(const Uuid playerId, const Uuid shipId) const
{
  const auto costs = m_repositories.shipPriceRepository->findAllByShip(shipId);
  debitResources(playerId, costs);

  const auto player       = m_repositories.playerRepository->findOneById(playerId);
  const auto shipTemplate = m_repositories.shipRepository->findOneById(shipId);

  PlayerShip ship{
    .faction     = shipTemplate.faction,
    .ship        = shipTemplate.id,
    .name        = shipTemplate.name,
    .player      = player.id,
    .active      = false,
    .hullPoints  = shipTemplate.maxHullPoints,
    .powerPoints = shipTemplate.maxPowerPoints,
  };

  m_repositories.playerShipRepository->save(ship);
}

namespace {
auto findExistingAmount(const std::vector<PlayerResource> &resources, const Uuid toFind)
  -> std::optional<float>
{
  for (const auto &resource : resources)
  {
    if (toFind == resource.resource)
    {
      return resource.amount;
    }
  }

  return {};
}
} // namespace

void PurchaseService::debitResources(const Uuid playerId,
                                     const std::unordered_map<Uuid, float> &costs) const
{
  const auto funds = m_repositories.playerResourceRepository->findAllByPlayer(playerId);

  for (const auto &[resource, amount] : costs)
  {
    const auto fund = findExistingAmount(funds, resource);
    auto sum        = -amount;
    if (fund)
    {
      sum += *fund;
    }

    PlayerResource data{
      .player   = playerId,
      .resource = resource,
      .amount   = sum,
    };

    m_repositories.playerResourceRepository->save(data);
  }
}

} // namespace bsgo
