
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "TargetMessage.hh"
#include "TargetPickedMessage.hh"

namespace bsgalone::server {

class TargetPickedMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  TargetPickedMessageConsumer(const Services &services,
                              core::IMessageQueue *const outputMessageQueue);
  ~TargetPickedMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void broadcastMessageToSystem(std::unique_ptr<core::TargetMessage> message);
};

} // namespace bsgalone::server
