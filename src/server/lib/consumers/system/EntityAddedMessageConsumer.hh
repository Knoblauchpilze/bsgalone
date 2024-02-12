
#pragma once

#include "AbstractMessageConsumer.hh"
#include "EntityAddedMessage.hh"
#include "Services.hh"

namespace bsgo {

class EntityAddedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityAddedMessageConsumer(const Services &services);
  ~EntityAddedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  EntityServiceShPtr m_entityService{};

  void handleShipAdded(const Uuid shipDbId, const Uuid systemDbId) const;
};

} // namespace bsgo
