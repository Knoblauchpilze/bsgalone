
#include "VelocityMessageConsumer.hh"
#include "VelocityMessage.hh"

namespace bsgo {

VelocityMessageConsumer::VelocityMessageConsumer(const Services &services,
                                                 IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("velocity", {MessageType::VELOCITY})
  , m_shipService(services.ship)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void VelocityMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &velocity = message.as<VelocityMessage>();

  const auto shipDbId     = velocity.getShipDbId();
  const auto acceleration = velocity.getAcceleration();

  if (!m_shipService->accelerateShip(shipDbId, acceleration))
  {
    warn("Failed to process velocity message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<VelocityMessage>(shipDbId, acceleration);
  out->validate();
  out->copyClientIdIfDefined(velocity);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
