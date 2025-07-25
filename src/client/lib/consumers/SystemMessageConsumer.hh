
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "EntityAddedMessage.hh"
#include "EntityRemovedMessage.hh"
#include "ScannedMessage.hh"
#include "Uuid.hh"

namespace pge {

class SystemMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  SystemMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                        bsgo::CoordinatorShPtr coordinator);
  ~SystemMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void handleScanOperation(const bsgo::ScannedMessage &message) const;
  void handleEntityAdded(const bsgo::EntityAddedMessage &message) const;
  void handleEntityRemoved(const bsgo::EntityRemovedMessage &message) const;

  void handleShipCreation(const bsgo::PlayerShipData &data) const;
  void handleAsteroidCreation(const bsgo::AsteroidData &data) const;
  void handlePlayerCreation(const bsgo::PlayerData &data) const;
};

} // namespace pge
