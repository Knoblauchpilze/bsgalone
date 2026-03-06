
#pragma once

#include "AbstractMessageConsumer.hh"
#include "AsteroidData.hh"
#include "IMessageQueue.hh"
#include "PlayerShipData.hh"
#include "Services.hh"

namespace bsgalone::server {

class EntityAddedMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  EntityAddedMessageConsumer(const Services &services,
                             core::IMessageQueue *const outputMessageQueue);
  ~EntityAddedMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  EntityServiceShPtr m_entityService{};
  LoadingServiceShPtr m_loadingService{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleShipAdded(const core::Uuid systemDbId, const core::PlayerShipData &data) const;
  void handleAsteroidAdded(const core::Uuid systemDbId, const core::AsteroidData &data) const;
};

} // namespace bsgalone::server
