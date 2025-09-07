
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
    warn("Failed to purchase item " + str(itemId) + " with type " + str(type) + " for player "
           + str(playerId),
         "Player can not afford it");
    return false;
  }
  if (!verifyPreconditions(playerId, itemId, type))
  {
    warn("Failed to purchase item " + str(itemId) + " with type " + str(type) + " for player "
           + str(playerId),
         "Preconditions are not met");
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

namespace {
auto buildPlayerResources(const Uuid playerDbId, const PlayerResourceRepository &repository)
  -> std::vector<PlayerResourceData>
{
  std::vector<PlayerResourceData> out;

  for (const auto &resource : repository.findAllByPlayer(playerDbId))
  {
    out.push_back(fromDbPlayerResource(resource));
  }

  return out;
}

auto buildWeapons(const WeaponRepository &repository, const WeaponPriceRepository &priceRepository)
  -> std::vector<WeaponData>
{
  std::vector<WeaponData> out;

  for (const auto &id : repository.findAll())
  {
    const auto weapon = repository.findOneById(id);
    out.push_back(fromDbWeapon(weapon, priceRepository));
  }

  return out;
}

auto buildComputers(const ComputerRepository &repository,
                    const ComputerPriceRepository &priceRepository) -> std::vector<ComputerData>
{
  std::vector<ComputerData> out;

  for (const auto &id : repository.findAll())
  {
    const auto computer = repository.findOneById(id);
    out.push_back(fromDbComputer(computer, priceRepository));
  }

  return out;
}

auto buildShips(const Faction faction,
                const ShipRepository &repository,
                const ShipPriceRepository &priceRepository) -> std::vector<ShipData>
{
  std::vector<ShipData> out;

  for (const auto &ship : repository.findAllByFaction(faction))
  {
    out.push_back(fromDbShip(ship, priceRepository));
  }

  return out;
}
} // namespace

bool PurchaseService::verifyAffordability(const Uuid playerId,
                                          const Uuid itemId,
                                          const Item &type) const
{
  const auto player = m_repositories.playerRepository->findOneById(playerId);

  AffordabilityData data{
    .playerId = playerId,
    .itemId   = itemId,
    .itemType = type,

    .playerResources = buildPlayerResources(playerId, *m_repositories.playerResourceRepository),
    .weapons = buildWeapons(*m_repositories.weaponRepository, *m_repositories.weaponPriceRepository),
    .computers = buildComputers(*m_repositories.computerRepository,
                                *m_repositories.computerPriceRepository),
    .ships     = buildShips(player.faction,
                        *m_repositories.shipRepository,
                        *m_repositories.shipPriceRepository),
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
    .ship        = shipTemplate.id,
    .name        = shipTemplate.name,
    .player      = player.id,
    .active      = false,
    .hullPoints  = shipTemplate.maxHullPoints,
    .powerPoints = shipTemplate.maxPowerPoints,
  };

  m_repositories.playerShipRepository->create(ship);
}

namespace {
auto findExistingAmount(const std::vector<PlayerResource> &resources, const Uuid toFind)
  -> std::optional<int>
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
                                     const std::unordered_map<Uuid, int> &costs) const
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
