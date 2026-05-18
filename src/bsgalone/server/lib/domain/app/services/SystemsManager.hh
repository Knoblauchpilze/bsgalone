
#pragma once

#include "EntityRegistry.hh"
#include "ForManagingEntity.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_map>

namespace bsgalone::server {

class SystemsManager
{
  public:
  /// @brief - Registers the input entity registry and attach it to the specified system.
  /// This operation will fail in case there's already an entry for the system. It is not
  /// thread safe andcalling it concurrently will lead to undefined behavior.
  /// @param systemDbId - the identifier of the system for which the registry should be
  /// registered
  /// @param entityRegistry - the registry for the system
  void registerSystem(const core::Uuid systemDbId, core::EntityRegistryShPtr entityRegistry);

  auto entityManagerFor(const core::Uuid systemDbId) -> ForManagingEntityShPtr;

  private:
  std::unordered_map<core::Uuid, ForManagingEntityShPtr> m_entityManagers{};
};

using SystemsManagerShPtr = std::shared_ptr<SystemsManager>;

} // namespace bsgalone::server
