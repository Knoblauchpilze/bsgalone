
#include "Systems.hh"

#include "AISystem.hh"
#include "BulletSystem.hh"
#include "ComputerSystem.hh"
#include "EffectSystem.hh"
#include "HealthSystem.hh"
#include "LootSystem.hh"
#include "MotionSystem.hh"
#include "NetworkSystem.hh"
#include "PowerSystem.hh"
#include "RemovalSystem.hh"
#include "StatusSystem.hh"
#include "TargetSystem.hh"
#include "WeaponSystem.hh"

namespace bsgo {

Systems::Systems(SystemsConfig &&config)
  : core::CoreObject("systems")
{
  setService("bsgo");

  initialize(std::move(config));
}

void Systems::update(Coordinator &coordinator, const chrono::TickData &data) const
{
  for (const auto &system : m_systems)
  {
    system->update(coordinator, data);
  }
}

namespace {
template<typename T>
void installQueues(T &system, const SystemsConfig &config)
{
  if (config.internalMessageQueue != nullptr)
  {
    system.installInternalMessageQueue(config.internalMessageQueue);
  }
  if (config.outputMessageQueue != nullptr)
  {
    system.installOutputMessageQueue(config.outputMessageQueue);
  }
}

template<typename T>
void createSystem(std::vector<ISystemPtr> &systems, const SystemsConfig &config)
{
  auto toCreate = std::make_unique<T>();
  if (config.ignoredSystems.contains(toCreate->type()))
  {
    return;
  }

  installQueues(*toCreate, config);

  systems.emplace_back(std::move(toCreate));
}
} // namespace

void Systems::initialize(SystemsConfig &&config)
{
  createSystem<StatusSystem>(m_systems, config);
  createSystem<ComputerSystem>(m_systems, config);
  createSystem<EffectSystem>(m_systems, config);
  createSystem<MotionSystem>(m_systems, config);
  createSystem<PowerSystem>(m_systems, config);
  createSystem<WeaponSystem>(m_systems, config);
  createSystem<BulletSystem>(m_systems, config);
  createSystem<AISystem>(m_systems, config);
  createSystem<LootSystem>(m_systems, config);
  createSystem<HealthSystem>(m_systems, config);
  createSystem<RemovalSystem>(m_systems, config);
  createSystem<TargetSystem>(m_systems, config);
  createSystem<NetworkSystem>(m_systems, config);
}

} // namespace bsgo
