
#include "EntityRegistry.hh"

namespace bsgalone::core {

auto EntityRegistry::createEntity() -> Uuid
{
  const auto entity = m_registry.create();
  const Uuid uuid;

  m_entityToId.emplace(uuid, entity);
  m_idToEntity.emplace(entity, uuid);

  return uuid;
}

void EntityRegistry::clear()
{
  m_entityToId.clear();
  m_idToEntity.clear();

  m_registry.clear();
}

} // namespace bsgalone::core
