
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "TargetMessage.hh"

namespace bsgo {

class TargetMessageConsumer : public AbstractMessageConsumer
{
  public:
  TargetMessageConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~TargetMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  IMessageQueue *const m_outputMessageQueue{};

  void broadcastMessageToSystem(std::unique_ptr<TargetMessage> message);
};

} // namespace bsgo
