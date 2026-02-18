
#include "VelocityMessageConsumer.hh"
#include "VelocityMessage.hh"

namespace bsgo {

VelocityMessageConsumer::VelocityMessageConsumer(
  const Services &services,
  bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("velocity", {bsgalone::core::MessageType::VELOCITY})
  , m_shipService(services.ship)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void VelocityMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &velocity = message.as<VelocityMessage>();

  const auto shipDbId     = velocity.getShipDbId();
  const auto acceleration = velocity.getAcceleration();

  if (!m_shipService->accelerateShip(shipDbId, acceleration))
  {
    warn("Failed to process velocity message for ship " + str(shipDbId));
  }
}

} // namespace bsgo
