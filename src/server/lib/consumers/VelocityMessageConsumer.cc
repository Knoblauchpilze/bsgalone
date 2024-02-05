
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
  const auto &velocityMessage = message.as<VelocityMessage>();

  const auto shipDbId     = velocityMessage.getShipDbId();
  const auto acceleration = velocityMessage.getAcceleration();

  if (!m_shipService->accelerateShip(shipDbId, acceleration))
  {
    warn("Failed to process velocity message for ship " + str(shipDbId));
  }
}

} // namespace bsgo
