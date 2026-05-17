
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
  SystemsManager();

  void registerSystem(const core::Uuid systemDbId, core::EntityRegistryShPtr entityRegistry);
  auto entityManagerFor(const core::Uuid systemDbId) -> ForManagingEntityShPtr;

  private:
  std::unordered_map<core::Uuid, ForManagingEntityShPtr> m_entityManagers{};
};

using SystemsManagerShPtr = std::shared_ptr<SystemsManager>;

} // namespace bsgalone::server
