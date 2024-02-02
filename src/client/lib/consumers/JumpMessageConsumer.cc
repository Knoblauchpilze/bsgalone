
#include "JumpMessageConsumer.hh"

namespace pge {

JumpMessageConsumer::JumpMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                                         bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("jump",
                                  {bsgo::MessageType::JUMP_REQUESTED,
                                   bsgo::MessageType::JUMP_CANCELLED})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void JumpMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::JUMP_REQUESTED:
      handleJumpRequested(message.as<bsgo::JumpRequestedMessage>());
      break;
    case bsgo::MessageType::JUMP_CANCELLED:
      handleJumpCancelled(message.as<bsgo::JumpCancelledMessage>());
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
  }
}

void JumpMessageConsumer::handleJumpRequested(const bsgo::JumpRequestedMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process jump requested message for ship " + bsgo::str(shipDbId));
    return;
  }

  auto ship         = m_coordinator->getEntity(*maybeShip);
  const auto status = ship.statusComp().status();

  const auto newStatus = bsgo::updateStatusForJump(status);
  ship.statusComp().setStatus(newStatus);
}

void JumpMessageConsumer::handleJumpCancelled(const bsgo::JumpCancelledMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process jump cancelled message for ship " + bsgo::str(shipDbId));
    return;
  }

  auto ship         = m_coordinator->getEntity(*maybeShip);
  const auto status = ship.statusComp().status();

  const auto newStatus = bsgo::updateStatusAfterJumpCancellation(status);
  ship.statusComp().setStatus(newStatus);
}

} // namespace pge
