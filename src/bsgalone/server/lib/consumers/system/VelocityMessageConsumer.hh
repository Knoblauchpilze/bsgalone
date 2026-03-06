
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgalone::server {

class VelocityMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  VelocityMessageConsumer(const Services &services, core::IMessageQueue *const outputMessageQueue);
  ~VelocityMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  core::IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgalone::server
