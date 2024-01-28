
#include "PlayerDataSource.hh"
#include "Coordinator.hh"

namespace bsgo {

PlayerDataSource::PlayerDataSource(const Repositories &repositories,
                                   const Uuid systemDbId,
                                   const std::optional<Uuid> &playerDbId)
  : utils::CoreObject("bsgo")
  , m_systemDbId(systemDbId)
  , m_playerDbId(playerDbId)
  , m_repositories(repositories)
{
  setService("data");
  addModule("player");
}

auto PlayerDataSource::getPlayerEntityId() const -> std::optional<Uuid>
{
  if (m_playerDbId)
  {
    const auto maybePlayerDbId = m_playerDbIdsToEntityIds.find(*m_playerDbId);
    if (maybePlayerDbId != m_playerDbIdsToEntityIds.cend())
    {
      return maybePlayerDbId->second;
    }
  }

  return {};
}

auto PlayerDataSource::initialize(Coordinator &coordinator) const -> PlayerDbIdsToEntityIds
{
  const auto players = m_repositories.playerRepository->findAllBySystem(m_systemDbId);
  for (const auto &id : players)
  {
    registerPlayer(coordinator, id);
  }

  return m_playerDbIdsToEntityIds;
}

void PlayerDataSource::registerPlayer(Coordinator &coordinator, const Uuid playerDbId) const
{
  const auto playerEntityId = coordinator.createEntity(EntityKind::PLAYER);

  const auto player = m_repositories.playerRepository->findOneById(playerDbId);
  coordinator.addName(playerEntityId, player.name);
  coordinator.addDbId(playerEntityId, playerDbId);

  const std::unordered_set<ComponentType> toSync{ComponentType::RESOURCE};
  coordinator.addNetwork(playerEntityId, toSync);

  const auto res = m_playerDbIdsToEntityIds.try_emplace(playerDbId, playerEntityId);
  if (!res.second)
  {
    error("Unable to register player " + str(playerDbId),
          "Player already is attached to entity " + str(res.first->second));
  }

  registerResources(coordinator, playerEntityId, playerDbId);
}

void PlayerDataSource::registerResources(Coordinator &coordinator,
                                         const Uuid playerEntityId,
                                         const Uuid playerDbId) const
{
  const auto resources = m_repositories.playerResourceRepository->findAllByPlayer(playerDbId);
  for (const auto &resource : resources)
  {
    coordinator.addResourceComponent(playerEntityId, resource.resource, resource.amount);
  }
}

} // namespace bsgo
