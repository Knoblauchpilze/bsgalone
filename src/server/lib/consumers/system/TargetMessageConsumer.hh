
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "TargetMessage.hh"

namespace bsgo {

class TargetMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  TargetMessageConsumer(const Services &services,
                        bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~TargetMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void broadcastMessageToSystem(std::unique_ptr<TargetMessage> message);
};

} // namespace bsgo
