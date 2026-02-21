
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "ShipSelectedMessage.hh"

namespace bsgo {

class ShipSelectedMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  ShipSelectedMessageConsumer(const Services &services,
                              bsgalone::core::IMessageQueue *const systemMessageQueue,
                              bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~ShipSelectedMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  bsgalone::core::IMessageQueue *const m_systemMessageQueue{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleShipSwitchRequest(const bsgalone::core::ShipSelectedMessage &message) const;

  void handleSuccessfulSwitch(const bsgalone::core::ShipSelectedMessage &message) const;
};

} // namespace bsgo
