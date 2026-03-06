
#include "InternalMessageConsumer.hh"

namespace bsgalone::client {

InternalMessageConsumer::InternalMessageConsumer(const core::DatabaseEntityMapper &entityMapper,
                                                 core::CoordinatorShPtr coordinator)
  : AbstractMessageConsumer("internal", {core::MessageType::VELOCITY})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void InternalMessageConsumer::onEventReceived(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::VELOCITY:
      handleVelocityChanged(message.as<core::VelocityMessage>());
      break;
    default:
      error("Unsupported message type " + str(message.type()));
  }
}

void InternalMessageConsumer::handleVelocityChanged(const core::VelocityMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  const auto maybePlayerShipDbId = m_entityMapper.tryGetPlayerShipDbId();
  if (!maybePlayerShipDbId)
  {
    error("Failed to process velocity message for ship " + core::str(shipDbId),
          "Player ship is not defined");
  }

  if (shipDbId != *maybePlayerShipDbId)
  {
    warn("Failed to process velocity message for ship " + core::str(shipDbId),
         "Ship is not controlled by the player");
    return;
  }

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process velocity message for ship " + core::str(shipDbId),
         "Can't find an entity with such an id");
    return;
  }

  auto ship = m_coordinator->getEntity(*maybeShip);

  auto &velocity = ship.velocityComp();
  velocity.accelerate(message.getAcceleration());
}

} // namespace bsgalone::client
