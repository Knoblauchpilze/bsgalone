
#pragma once

#include "EntityRegistry.hh"
#include "Repositories.hh"
#include "System.hh"
#include "SystemProcessor.hh"

namespace bsgalone::server {

class SystemInitializer
{
  public:
  auto initializeSystem(const System &system) -> core::SystemProcessorShPtr;

  private:
  Repositories m_repositories{};

  void registerAsteroids(const core::Uuid &systemDbId, core::EntityRegistryShPtr registry);
};

} // namespace bsgalone::server
