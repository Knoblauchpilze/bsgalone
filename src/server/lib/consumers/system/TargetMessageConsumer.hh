
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class TargetMessageConsumer : public AbstractMessageConsumer
{
  public:
  TargetMessageConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~TargetMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgo
