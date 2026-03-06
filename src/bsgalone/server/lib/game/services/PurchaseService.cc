
#include "PurchaseService.hh"
#include "PurchaseUtils.hh"

namespace bsgalone::server {

PurchaseService::PurchaseService(const core::Repositories &repositories)
  : AbstractService("purchase", repositories)
{}

bool PurchaseService::tryPurchase(const core::Uuid playerId,
                                  const core::Uuid itemId,
                                  const core::Item &type) const
{
  if (!verifyAffordability(playerId, itemId, type))
  {
    warn("Failed to purchase item " + core::str(itemId) + " with type " + str(type) + " for player "
           + core::str(playerId),
         "Player can not afford it");
    return false;
  }
  if (!verifyPreconditions(playerId, itemId, type))
  {
    warn("Failed to purchase item " + core::str(itemId) + " with type " + str(type) + " for player "
           + core::str(playerId),
         "Preconditions are not met");
    return false;
  }

  switch (type)
  {
    case core::Item::WEAPON:
      tryPurchaseWeapon(playerId, itemId);
      break;
    case core::Item::COMPUTER:
      tryPurchaseComputer(playerId, itemId);
      break;
    case core::Item::SHIP:
      tryPurchaseShip(playerId, itemId);
      break;
    default:
      error("Invalid kind of item to buy", "Unsupported item " + str(type));
      break;
  }

  return true;
}

namespace {
auto buildPlayerResources(const core::Uuid playerDbId,
                          const core::PlayerResourceRepository &repository)
  -> std::vector<core::PlayerResourceData>
{
  std::vector<core::PlayerResourceData> out;

  for (const auto &resource : repository.findAllByPlayer(playerDbId))
  {
    out.push_back(fromDbPlayerResource(resource));
  }

  return out;
}

auto buildWeapons(const core::WeaponRepository &repository,
                  const core::WeaponPriceRepository &priceRepository)
  -> std::vector<core::WeaponData>
{
  std::vector<core::WeaponData> out;

  for (const auto &id : repository.findAll())
  {
    const auto weapon = repository.findOneById(id);
    out.push_back(fromDbWeapon(weapon, priceRepository));
  }

  return out;
}

auto buildComputers(const core::ComputerRepository &repository,
                    const core::ComputerPriceRepository &priceRepository)
  -> std::vector<core::ComputerData>
{
  std::vector<core::ComputerData> out;

  for (const auto &id : repository.findAll())
  {
    const auto computer = repository.findOneById(id);
    out.push_back(fromDbComputer(computer, priceRepository));
  }

  return out;
}

auto buildShips(const core::Faction faction,
                const core::ShipRepository &repository,
                const core::ShipPriceRepository &priceRepository) -> std::vector<core::ShipData>
{
  std::vector<core::ShipData> out;

  for (const auto &ship : repository.findAllByFaction(faction))
  {
    out.push_back(fromDbShip(ship, priceRepository));
  }

  return out;
}
} // namespace

bool PurchaseService::verifyAffordability(const core::Uuid playerId,
                                          const core::Uuid itemId,
                                          const core::Item &type) const
{
  const auto player = m_repositories.playerRepository->findOneById(playerId);

  core::AffordabilityData data{
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

bool PurchaseService::verifyPreconditions(const core::Uuid playerId,
                                          const core::Uuid itemId,
                                          const core::Item &type) const
{
  if (core::Item::SHIP == type)
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

void PurchaseService::tryPurchaseWeapon(const core::Uuid playerId, const core::Uuid weaponId) const
{
  const auto costs = m_repositories.weaponPriceRepository->findAllByWeapon(weaponId);
  debitResources(playerId, costs);

  core::PlayerWeapon weapon{
    .weapon = weaponId,
    .player = playerId,
  };
  m_repositories.playerWeaponRepository->save(weapon);
}

void PurchaseService::tryPurchaseComputer(const core::Uuid playerId,
                                          const core::Uuid computerId) const
{
  const auto costs = m_repositories.computerPriceRepository->findAllByComputer(computerId);
  debitResources(playerId, costs);

  core::PlayerComputer computer{
    .computer = computerId,
    .player   = playerId,
  };
  m_repositories.playerComputerRepository->save(computer);
}

void PurchaseService::tryPurchaseShip(const core::Uuid playerId, const core::Uuid shipId) const
{
  const auto costs = m_repositories.shipPriceRepository->findAllByShip(shipId);
  debitResources(playerId, costs);

  const auto player       = m_repositories.playerRepository->findOneById(playerId);
  const auto shipTemplate = m_repositories.shipRepository->findOneById(shipId);

  core::PlayerShip ship{
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
auto findExistingAmount(const std::vector<core::PlayerResource> &resources, const core::Uuid toFind)
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

void PurchaseService::debitResources(const core::Uuid playerId,
                                     const std::unordered_map<core::Uuid, int> &costs) const
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

    core::PlayerResource data{
      .player   = playerId,
      .resource = resource,
      .amount   = sum,
    };

    m_repositories.playerResourceRepository->save(data);
  }
}

} // namespace bsgalone::server
