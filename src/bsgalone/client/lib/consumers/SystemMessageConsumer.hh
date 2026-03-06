
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "EntityAddedMessage.hh"
#include "EntityRemovedMessage.hh"
#include "ScannedMessage.hh"
#include "Uuid.hh"

namespace bsgalone::client {

class SystemMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  SystemMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                        core::CoordinatorShPtr coordinator);
  ~SystemMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  core::DatabaseEntityMapper &m_entityMapper;
  core::CoordinatorShPtr m_coordinator{};

  void handleScanOperation(const core::ScannedMessage &message) const;
  void handleEntityAdded(const core::EntityAddedMessage &message) const;
  void handleEntityRemoved(const core::EntityRemovedMessage &message) const;

  void handleShipCreation(const core::PlayerShipData &data) const;
  void handleAsteroidCreation(const core::AsteroidData &data) const;
  void handlePlayerCreation(const core::PlayerData &data) const;
};

} // namespace bsgalone::client
