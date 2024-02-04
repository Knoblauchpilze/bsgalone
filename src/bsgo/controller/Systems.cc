
#include "Systems.hh"

#include "AISystem.hh"
#include "BulletSystem.hh"
#include "ComputerSystem.hh"
#include "EffectSystem.hh"
#include "HealthSystem.hh"
#include "LootSystem.hh"
#include "MotionSystem.hh"
#include "NetworkSystem.hh"
#include "OwnerSystem.hh"
#include "PowerSystem.hh"
#include "StatusSystem.hh"
#include "TargetSystem.hh"
#include "WeaponEffectComponent.hh"
#include "WeaponSystem.hh"

namespace bsgo {

Systems::Systems(SystemsConfig &&config)
  : utils::CoreObject("systems")
{
  setService("bsgo");

  initialize(std::move(config));
}

void Systems::update(Coordinator &coordinator, const float elapsedSeconds) const
{
  for (const auto &system : m_systems)
  {
    system->update(coordinator, elapsedSeconds);
  }
}

void Systems::initialize(SystemsConfig &&config)
{
  auto status = std::make_unique<StatusSystem>();
  m_systems.push_back(std::move(status));

  auto computer = std::make_unique<ComputerSystem>();
  m_systems.push_back(std::move(computer));

  auto effect = std::make_unique<EffectSystem>();
  m_systems.push_back(std::move(effect));

  auto motion = std::make_unique<MotionSystem>();
  m_systems.push_back(std::move(motion));

  auto power = std::make_unique<PowerSystem>();
  m_systems.push_back(std::move(power));

  auto weapon = std::make_unique<WeaponSystem>();
  m_systems.push_back(std::move(weapon));

  auto bullet = std::make_unique<BulletSystem>();
  m_systems.push_back(std::move(bullet));

  auto ai = std::make_unique<AISystem>();
  m_systems.push_back(std::move(ai));

  auto target = std::make_unique<TargetSystem>();
  m_systems.push_back(std::move(target));

  auto loot = std::make_unique<LootSystem>();
  m_systems.push_back(std::move(loot));

  auto health = std::make_unique<HealthSystem>();
  m_systems.push_back(std::move(health));

  auto owner = std::make_unique<OwnerSystem>();
  m_systems.push_back(std::move(owner));

  m_systems.emplace_back(std::move(config.networkSystem));

  for (auto &system : m_systems)
  {
    system->installInternalMessageQueue(config.internalMessageQueue);
    system->installOutputMessageQueue(config.outputMessageQueue);
  }
}

} // namespace bsgo
