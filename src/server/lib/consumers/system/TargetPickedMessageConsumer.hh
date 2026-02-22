
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "TargetMessage.hh"
#include "TargetPickedMessage.hh"

namespace bsgo {

class TargetPickedMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  TargetPickedMessageConsumer(const Services &services,
                              bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~TargetPickedMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void broadcastMessageToSystem(std::unique_ptr<TargetMessage> message);
};

} // namespace bsgo
