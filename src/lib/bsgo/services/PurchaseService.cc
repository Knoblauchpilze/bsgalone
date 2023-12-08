
#include "PurchaseService.hh"

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

bool PurchaseService::tryPurchase(const Uuid &id, const Item &type)
{
  checkPlayerDbIdExists();

  bool result{false};
  switch (type)
  {
    case Item::COMPUTER:
      result = m_repositories.computerRepository->saveForPlayer(*m_playerDbId, id);
      break;
    case Item::WEAPON:
      result = m_repositories.weaponRepository->saveForPlayer(*m_playerDbId, id);
      break;
    default:
      error("Invalid kind of item to buy", "Unsupported item " + str(type));
      break;
  }

  return result;
}

void PurchaseService::checkPlayerDbIdExists() const
{
  if (!m_playerDbId)
  {
    error("Expected player db id to exist but it does not");
  }
}

} // namespace bsgo
