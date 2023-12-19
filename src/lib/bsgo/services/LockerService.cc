
#include "LockerService.hh"

namespace bsgo {

LockerService::LockerService(const Repositories &repositories)
  : AbstractService("locker", repositories)
{}

void LockerService::setPlayerDbId(const Uuid &player)
{
  m_playerDbId = player;
}

bool LockerService::isReady() const noexcept
{
  return m_playerDbId.has_value();
}

void LockerService::checkPlayerDbIdExists() const
{
  if (!m_playerDbId)
  {
    error("Expected player db id to exist but it does not");
  }
}

} // namespace bsgo
