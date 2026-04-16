
#include "EntityRegistry.hh"

namespace bsgalone::core {

auto EntityRegistry::createEntity() -> Uuid
{
  const auto entity = m_registry.create();
  const auto uuid   = Uuid::random();

  m_entities.emplace(uuid, entity);

  return uuid;
}

} // namespace bsgalone::core
