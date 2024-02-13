
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class EntityAddedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityAddedMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~EntityAddedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  EntityServiceShPtr m_entityService{};
  IMessageQueue *const m_messageQueue{};

  void handleShipAdded(const Uuid shipDbId, const Uuid systemDbId) const;
};

} // namespace bsgo
