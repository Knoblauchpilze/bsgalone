
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class VelocityMessageConsumer : public AbstractMessageConsumer
{
  public:
  VelocityMessageConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~VelocityMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgo
