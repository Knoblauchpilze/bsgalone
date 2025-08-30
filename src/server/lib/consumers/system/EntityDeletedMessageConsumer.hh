
#pragma once

#include "AbstractMessageConsumer.hh"
#include "EntityRemovedMessage.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

/// @brief - This consumer is added to the system processors consumers and is
/// responsible to handle the entity added/removed messages produced by the
/// internal consumer. This means deleting the entity from the system processor
/// and forwarding the messages to the client applications.
class EntityDeletedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityDeletedMessageConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~EntityDeletedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  EntityServiceShPtr m_entityService{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleShipRemoved(const EntityRemovedMessage &message) const;
  void handleAsteroidRemoved(const EntityRemovedMessage &message) const;
};

} // namespace bsgo
