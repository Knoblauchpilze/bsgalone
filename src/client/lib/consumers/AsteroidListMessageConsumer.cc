
#include "AsteroidListMessageConsumer.hh"
#include "AsteroidListMessage.hh"
#include "MessageUtils.hh"

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

void AsteroidListMessageConsumer::registerAsteroid(const bsgo::AsteroidData & /*data*/) const
{
  warn("should load asteroid");
}

} // namespace pge
