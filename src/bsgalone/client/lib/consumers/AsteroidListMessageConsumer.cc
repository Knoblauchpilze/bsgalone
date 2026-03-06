
#include "AsteroidListMessageConsumer.hh"
#include "AsteroidDataSource.hh"
#include "AsteroidListMessage.hh"

namespace bsgalone::client {

AsteroidListMessageConsumer::AsteroidListMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                                                         core::CoordinatorShPtr coordinator)
  : AbstractGameMessageConsumer("asteroid", core::MessageType::ASTEROID_LIST)
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void AsteroidListMessageConsumer::onMessageReceivedInternal(const core::IMessage &message)
{
  const auto asteroidsList = message.as<core::AsteroidListMessage>();

  for (const auto &asteroidData : asteroidsList.getAsteroidsData())
  {
    registerAsteroid(asteroidData);
  }
}

void AsteroidListMessageConsumer::registerAsteroid(const core::AsteroidData &data) const
{
  core::AsteroidDataSource source;
  source.registerAsteroid(*m_coordinator, data, m_entityMapper);
}

} // namespace bsgalone::client
