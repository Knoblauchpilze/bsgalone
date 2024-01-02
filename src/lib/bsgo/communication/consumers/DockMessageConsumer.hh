
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class DockMessageConsumer : public AbstractMessageConsumer
{
  public:
  DockMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~DockMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  IMessageQueue *const m_messageQueue{};

  void handleDocking(const Uuid &shipDbId, const Uuid &shipEntityId) const;
};

} // namespace bsgo
