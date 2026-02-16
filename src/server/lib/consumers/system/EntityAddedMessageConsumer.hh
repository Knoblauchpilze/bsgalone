
#pragma once

#include "AbstractMessageConsumer.hh"
#include "AsteroidData.hh"
#include "IMessageQueue.hh"
#include "PlayerShipData.hh"
#include "Services.hh"

namespace bsgo {

class EntityAddedMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  EntityAddedMessageConsumer(const Services &services,
                             bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~EntityAddedMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  EntityServiceShPtr m_entityService{};
  LoadingServiceShPtr m_loadingService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleShipAdded(const Uuid systemDbId, const PlayerShipData &data) const;
  void handleAsteroidAdded(const Uuid systemDbId, const AsteroidData &data) const;
};

} // namespace bsgo
