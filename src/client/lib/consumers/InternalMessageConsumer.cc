
#include "InternalMessageConsumer.hh"

namespace pge {

InternalMessageConsumer::InternalMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                                                 bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("internal", {bsgo::MessageType::VELOCITY})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void InternalMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::VELOCITY:
      handleVelocityChanged(message.as<bsgo::VelocityMessage>());
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
  }
}

void InternalMessageConsumer::handleVelocityChanged(const bsgo::VelocityMessage &message) const
{
  const auto shipDbId = message.getShipDbId();
  if (shipDbId != m_entityMapper.playerShipDbId())
  {
    warn("Failed to process jump requested message for ship " + bsgo::str(shipDbId),
         "Ship is not controlled by the player");
    return;
  }

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process jump requested message for ship " + bsgo::str(shipDbId),
         "Can't find an entity with such an id");
    return;
  }

  auto ship = m_coordinator->getEntity(*maybeShip);

  auto &velocity = ship.velocityComp();
  velocity.accelerate(message.getAcceleration());
}

} // namespace pge
