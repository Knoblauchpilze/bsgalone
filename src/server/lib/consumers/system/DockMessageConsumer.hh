
#pragma once

#include "AbstractMessageConsumer.hh"
#include "DockMessage.hh"
#include "IMessageQueue.hh"
#include "LoadingTransition.hh"
#include "Services.hh"

namespace bsgo {

class DockMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  DockMessageConsumer(const Services &services,
                      bsgalone::core::IMessageQueue *const systemMessageQueue,
                      bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~DockMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  EntityServiceShPtr m_entityService{};
  bsgalone::core::IMessageQueue *const m_systemMessageQueue{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleDocking(const bsgalone::core::DockMessage &message) const;
  void handleUndocking(const bsgalone::core::DockMessage &message) const;
  void handleReturnToOutpost(const bsgalone::core::DockMessage &message) const;

  void publishLoadingMessages(const LoadingTransition transition, const Uuid shipDbId) const;
};

} // namespace bsgo
