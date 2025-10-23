
#pragma once

#include "AbstractMessageConsumer.hh"
#include "AsteroidData.hh"
#include "IMessageQueue.hh"
#include "PlayerShipData.hh"
#include "Services.hh"

namespace bsgo {

class EntityAddedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityAddedMessageConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~EntityAddedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  EntityServiceShPtr m_entityService{};
  LoadingServiceShPtr m_loadingService{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleShipAdded(const Uuid systemDbId, const PlayerShipData &data) const;
  void handleAsteroidAdded(const Uuid systemDbId, const AsteroidData &data) const;
};

} // namespace bsgo
