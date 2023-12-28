
#include "PurchaseService.hh"
#include "PurchaseUtils.hh"

namespace bsgo {

PurchaseService::PurchaseService(const Repositories &repositories)
  : AbstractService("purchase", repositories)
{}

void PurchaseService::setPlayerDbId(const Uuid &player)
{
  m_playerDbId = player;
}

bool PurchaseService::isReady() const noexcept
{
  return m_playerDbId.has_value();
}

bool PurchaseService::tryPurchase(const Uuid &id, const Item &type) const
{
  checkPlayerDbIdExists();

  if (!verifyAffordability(id, type))
  {
    return false;
  }
  if (!verifyPreconditions(id, type))
  {
    return false;
  }

  switch (type)
  {
    case Item::WEAPON:
      tryPurchaseWeapon(id);
      break;
    case Item::COMPUTER:
      tryPurchaseComputer(id);
      break;
    case Item::SHIP:
      tryPurchaseShip(id);
      break;
    default:
      error("Invalid kind of item to buy", "Unsupported item " + str(type));
      break;
  }

  return true;
}

void PurchaseService::checkPlayerDbIdExists() const
{
  if (!m_playerDbId)
  {
    error("Expected player db id to exist but it does not");
  }
}

bool PurchaseService::verifyAffordability(const Uuid &id, const Item &type) const
{
  AffordabilityData data{
    .playerId          = *m_playerDbId,
    .itemId            = id,
    .itemType          = type,
    .resourceRepo      = m_repositories.playerResourceRepository,
    .weaponPriceRepo   = m_repositories.weaponPriceRepository,
    .computerPriceRepo = m_repositories.computerPriceRepository,
    .shipPriceRepo     = m_repositories.shipPriceRepository,
  };

  const auto affordability = computeAffordability(data);
  return affordability.canAfford;
}

bool PurchaseService::verifyPreconditions(const Uuid &id, const Item &type) const
{
  if (Item::SHIP == type)
  {
    const auto shipsIds = m_repositories.playerShipRepository->findAllByPlayer(*m_playerDbId);
    for (const auto &shipId : shipsIds)
    {
      const auto ship = m_repositories.playerShipRepository->findOneById(shipId);
      if (id == ship.ship)
      {
        return false;
      }
    }
  }

  return true;
}

void PurchaseService::tryPurchaseWeapon(const Uuid &id) const
{
  const auto costs = m_repositories.weaponPriceRepository->findAllByWeapon(id);
  debitResources(costs);

  PlayerWeapon weapon{
    .weapon = id,
    .player = *m_playerDbId,
  };
  m_repositories.playerWeaponRepository->save(weapon);
}

void PurchaseService::tryPurchaseComputer(const Uuid &id) const
{
  const auto costs = m_repositories.computerPriceRepository->findAllByComputer(id);
  debitResources(costs);

  PlayerComputer computer{
    .computer = id,
    .player   = *m_playerDbId,
  };
  m_repositories.playerComputerRepository->save(computer);
}

void PurchaseService::tryPurchaseShip(const Uuid &id) const
{
  const auto costs = m_repositories.shipPriceRepository->findAllByShip(id);
  debitResources(costs);

  const auto player       = m_repositories.playerRepository->findOneById(*m_playerDbId);
  const auto shipTemplate = m_repositories.shipRepository->findOneById(id);

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
auto findExistingAmount(const std::vector<PlayerResource> &resources, const Uuid &toFind)
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

void PurchaseService::debitResources(const std::unordered_map<Uuid, float> &costs) const
{
  const auto funds = m_repositories.playerResourceRepository->findAllByPlayer(*m_playerDbId);

  for (const auto &[resource, amount] : costs)
  {
    const auto fund = findExistingAmount(funds, resource);
    auto sum        = -amount;
    if (fund)
    {
      sum += *fund;
    }

    PlayerResource data{
      .player   = *m_playerDbId,
      .resource = resource,
      .amount   = sum,
    };

    m_repositories.playerResourceRepository->save(data);
  }
}

} // namespace bsgo
