
#pragma once

#include "AbstractMessageConsumer.hh"
#include "EntityRemovedMessage.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgalone::server {

/// @brief - This consumer is added to the system processors consumers and is
/// responsible to handle the entity added/removed messages produced by the
/// internal consumer. This means deleting the entity from the system processor
/// and forwarding the messages to the client applications.
class EntityDeletedMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  EntityDeletedMessageConsumer(const Services &services,
                               core::IMessageQueue *const outputMessageQueue);
  ~EntityDeletedMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  EntityServiceShPtr m_entityService{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleShipRemoved(const core::EntityRemovedMessage &message) const;
  void handleAsteroidRemoved(const core::EntityRemovedMessage &message) const;
};

} // namespace bsgalone::server
