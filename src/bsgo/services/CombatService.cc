
#include "CombatService.hh"

namespace bsgo {

CombatService::CombatService(const Repositories &repositories)
  : AbstractService("combat", repositories)
{}

bool CombatService::tryDistributeResource(const Uuid playerDbId,
                                          const Uuid resourceDbId,
                                          const float amount) const
{
  const auto player = m_repositories.playerRepository->findOneById(playerDbId);

  const auto existing = findExistingResourceAmount(playerDbId, resourceDbId);
  const auto updated  = existing + amount;
  if (updated < 0.0f)
  {
    return false;
  }

  PlayerResource data{.player = playerDbId, .resource = resourceDbId, .amount = updated};
  m_repositories.playerResourceRepository->save(data);

  return true;
}

auto CombatService::findExistingResourceAmount(const Uuid playerDbId, const Uuid resourceDbId) const
  -> float
{
  const auto resources = m_repositories.playerResourceRepository->findAllByPlayer(playerDbId);
  for (const auto &resource : resources)
  {
    if (resource.resource == resourceDbId)
    {
      return resource.amount;
    }
  }

  return 0.0f;
}

} // namespace bsgo
