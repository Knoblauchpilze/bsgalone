
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

} // namespace bsgalone::core
