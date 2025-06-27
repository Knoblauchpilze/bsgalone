
#pragma once

#include "AbstractMessageConsumer.hh"
#include "DockMessage.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class DockMessageConsumer : public AbstractMessageConsumer
{
  public:
  DockMessageConsumer(const Services &services,
                      IMessageQueue *const systemMessageQueue,
                      IMessageQueue *const messageQueue);
  ~DockMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  EntityServiceShPtr m_entityService{};
  IMessageQueue *const m_systemMessageQueue{};
  IMessageQueue *const m_messageQueue{};

  void handleDocking(const DockMessage &message) const;
  void handleUndocking(const DockMessage &message) const;
};

} // namespace bsgo
