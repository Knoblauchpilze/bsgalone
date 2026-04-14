
#pragma once

#include "Uuid.hh"
#include <entt/entt.hpp>
#include <unordered_map>

namespace bsgalone::core {

class EntityRegistry
{
  public:
  EntityRegistry()  = default;
  ~EntityRegistry() = default;

  /// @brief - Creates a new (empty) entity and returns it. The returned identifier can
  /// be used later on to modify the entity for example when adding components.
  /// This is currently not thread-safe.
  /// @return - the identifier of the entity
  auto createEntity() -> Uuid;

  /// @brief - Adds a component to the entity. If the entity does not exist, this function
  /// will raise an error.
  /// @param entityId - the entity to add the component to
  /// @param component - the component to add to the entity
  template<typename Component>
  void addComponent(const Uuid entityId, Component &&component);

  /// @brief - Applied the function to all the entities having all required components set.
  /// @tparam ...Components - the required components for an entity to qualify for the
  /// application of the function
  /// @tparam Func - the signature of the function: this parameter should be inferred from
  /// the signature of of the lambda provided and does not need to be provided explicitly
  /// in general.
  /// @param modifier - a function to update components. Can be provided as a lambda
  template<typename... Components, typename Func>
  void apply(Func &&modifier);

  private:
  /// @brief - The registry used by entt to store entities.
  entt::registry m_registry{};

  /// @brief - A table of association between the identifier of the entities as defined
  /// by `entt` to agnostic identifier that can be communicated to the rest of the app
  /// without leaking the library details.
  std::unordered_map<Uuid, entt::entity> m_entities{};

  Uuid m_nextEntity{Uuid(0)};
};

} // namespace bsgalone::core

#include "EntityRegistry.hxx"
