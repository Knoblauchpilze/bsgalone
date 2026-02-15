
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class VelocityMessageConsumer : public AbstractMessageConsumer
{
  public:
  VelocityMessageConsumer(const Services &services,
                          bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~VelocityMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgo
