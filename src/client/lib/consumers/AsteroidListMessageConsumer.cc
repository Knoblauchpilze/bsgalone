
#include "AsteroidListMessageConsumer.hh"
#include "AsteroidDataSource.hh"
#include "AsteroidListMessage.hh"

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
  bsgo::AsteroidDataSource source;
  source.registerAsteroid(*m_coordinator, data, m_entityMapper);
}

} // namespace pge
