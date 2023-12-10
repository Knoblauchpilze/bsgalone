
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

  switch (type)
  {
    case Item::WEAPON:
      tryPurchaseWeapon(id);
      break;
    case Item::COMPUTER:
      tryPurchaseComputer(id);
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
  };

  const auto affordability = computeAffordability(data);
  return affordability.canAfford;
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

void PurchaseService::debitResources(const std::unordered_map<Uuid, float> &costs) const
{
  for (const auto &[resource, amount] : costs)
  {
    PlayerResource data{
      .player   = *m_playerDbId,
      .resource = resource,
      .amount   = -amount,
    };

    m_repositories.playerResourceRepository->save(data);
  }
}

} // namespace bsgo
