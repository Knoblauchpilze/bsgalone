
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class HangarMessageConsumer : public AbstractMessageConsumer
{
  public:
  HangarMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~HangarMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  IMessageQueue *const m_messageQueue{};

  void handleShipSwitchRequest(const Uuid &shipDbId) const;
};

} // namespace bsgo
