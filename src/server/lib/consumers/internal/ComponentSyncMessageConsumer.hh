
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ComponentSyncMessage.hh"
#include "IMessageQueue.hh"
#include "SystemProcessor.hh"
#include "SystemService.hh"
#include <unordered_map>

namespace bsgo {

class ComponentSyncMessageConsumer : public AbstractMessageConsumer
{
  public:
  ComponentSyncMessageConsumer(SystemServiceShPtr systemService,
                               SystemProcessorMap systemProcessors,
                               IMessageQueue *const outputMessageQueue);
  ~ComponentSyncMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  SystemProcessorMap m_systemProcessors{};
  IMessageQueue *const m_outputMessageQueue{};

  auto determineSystemForShip(const Uuid shipDbId) const -> Uuid;
  auto determineSystemForAsteroid(const Uuid asteroidDbId) const -> Uuid;
  auto determineSystemForOutpost(const Uuid outpostDbId) const -> Uuid;
};

} // namespace bsgo
