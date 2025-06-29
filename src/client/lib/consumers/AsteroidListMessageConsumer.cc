
#include "AsteroidListMessageConsumer.hh"
#include "AsteroidListMessage.hh"

#include "CircleBox.hh"

namespace pge {

AsteroidListMessageConsumer::AsteroidListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                                         bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("asteroid", {bsgo::MessageType::ASTEROID_LIST})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void AsteroidListMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  const auto asteroidsList = message.as<bsgo::AsteroidListMessage>();

  for (const auto &asteroidData : asteroidsList.getAsteroidsData())
  {
    registerAsteroid(asteroidData);
  }
}

void AsteroidListMessageConsumer::registerAsteroid(const bsgo::AsteroidData &data) const
{
  // TODO: Should probably be put in a common place to avoid duplication
  // with AsteroidDataSource::registerAsteroid
  const auto asteroidEntityId = m_coordinator->createEntity(bsgo::EntityKind::ASTEROID);

  m_coordinator->addDbId(asteroidEntityId, data.dbId);
  auto box = std::make_unique<bsgo::CircleBox>(data.position, data.radius);
  m_coordinator->addTransform(asteroidEntityId, std::move(box));
  m_coordinator->addHealth(asteroidEntityId, data.health, data.health, 0.0f);
  m_coordinator->addRemoval(asteroidEntityId);
  m_coordinator->addScanned(asteroidEntityId);
  if (data.resource && data.amount)
  {
    m_coordinator->addLoot(asteroidEntityId);
    m_coordinator->addResourceComponent(asteroidEntityId, *data.resource, *data.amount);
  }
  m_coordinator->addNetwork(asteroidEntityId, {bsgo::ComponentType::HEALTH});

  m_entityMapper.registerAsteroid(data.dbId, asteroidEntityId);
}

} // namespace pge
