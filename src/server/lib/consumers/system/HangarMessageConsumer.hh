
#pragma once

#include "AbstractMessageConsumer.hh"
#include "HangarMessage.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class HangarMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  HangarMessageConsumer(const Services &services,
                        bsgalone::core::IMessageQueue *const systemMessageQueue,
                        bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~HangarMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  bsgalone::core::IMessageQueue *const m_systemMessageQueue{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleShipSwitchRequest(const HangarMessage &message) const;

  void handleSuccessfulSwitch(const HangarMessage &message) const;
};

} // namespace bsgo
