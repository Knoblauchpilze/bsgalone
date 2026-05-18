
#include "SystemsManager.hh"
#include "EntityManager.hh"

namespace bsgalone::server {

void SystemsManager::registerSystem(const core::Uuid systemDbId,
                                    core::EntityRegistryShPtr entityRegistry)
{
  const auto maybeManager = m_entityManagers.find(systemDbId);
  if (maybeManager != m_entityManagers.end())
  {
    throw std::invalid_argument("Overriding existing manager for system " + systemDbId.str());
  }

  m_entityManagers[systemDbId] = std::make_shared<EntityManager>(std::move(entityRegistry));
}

auto SystemsManager::entityManagerFor(const core::Uuid systemDbId) -> ForManagingEntityShPtr
{
  const auto maybeManager = m_entityManagers.find(systemDbId);
  if (maybeManager == m_entityManagers.end())
  {
    throw std::invalid_argument("No such system " + systemDbId.str());
  }

  return maybeManager->second;
}

} // namespace bsgalone::server
