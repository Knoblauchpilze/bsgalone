
#pragma once

#include "EntityRegistry.hh"

namespace bsgalone::core {

template<typename Component>
inline void EntityRegistry::addComponent(const Uuid entityId, Component &&component)
{
  const auto maybeEntity = m_entities.find(entityId);
  if (maybeEntity == m_entities.end())
  {
    throw std::invalid_argument("No such entity " + str(entityId));
  }

  m_registry.emplace<Component>(maybeEntity->second, std::forward<Component>(component));
}

template<typename... Components, typename Func>
inline void EntityRegistry::apply(Func &&modifier)
{
  auto view = m_registry.view<Components...>();
  view.each(std::forward<Func>(modifier));
}

} // namespace bsgalone::core
