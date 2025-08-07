
#pragma once

#include "AbstractMessageConsumer.hh"
#include "HangarMessage.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class HangarMessageConsumer : public AbstractMessageConsumer
{
  public:
  HangarMessageConsumer(const Services &services,
                        IMessageQueue *const systemMessageQueue,
                        IMessageQueue *const outputMessageQueue);
  ~HangarMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  IMessageQueue *const m_systemMessageQueue{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleShipSwitchRequest(const HangarMessage &message) const;
};

} // namespace bsgo
