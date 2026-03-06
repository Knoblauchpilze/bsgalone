
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "ShipSelectedMessage.hh"

namespace bsgalone::server {

class ShipSelectedMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  ShipSelectedMessageConsumer(const Services &services,
                              core::IMessageQueue *const systemMessageQueue,
                              core::IMessageQueue *const outputMessageQueue);
  ~ShipSelectedMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  core::IMessageQueue *const m_systemMessageQueue{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleShipSwitchRequest(const core::ShipSelectedMessage &message) const;

  void handleSuccessfulSwitch(const core::ShipSelectedMessage &message) const;
};

} // namespace bsgalone::server
