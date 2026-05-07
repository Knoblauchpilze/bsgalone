
#pragma once

#include "EntityRegistry.hh"

namespace bsgalone::core {

template<typename Component>
inline void EntityRegistry::addComponent(const Uuid entityId, Component &&component)
{
  const auto maybeEntity = m_entityToId.find(entityId);
  if (maybeEntity == m_entityToId.end())
  {
    throw std::invalid_argument("No such entity " + entityId.str());
  }

  m_registry.emplace<Component>(maybeEntity->second, std::forward<Component>(component));
}

template<typename... Components, typename Func>
inline void EntityRegistry::apply(Func &&modifier)
{
  auto view = m_registry.view<Components...>();
  view.each(std::forward<Func>(modifier));
}

template<typename... Components, typename Func>
inline void EntityRegistry::applyWithId(Func &&modifier)
{
  auto view = m_registry.view<Components...>();
  view.each([&](entt::entity entity, Components &...comps) {
    modifier(m_idToEntity.at(entity), comps...);
  });
}

} // namespace bsgalone::core
