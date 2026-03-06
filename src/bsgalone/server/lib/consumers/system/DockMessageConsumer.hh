
#pragma once

#include "AbstractMessageConsumer.hh"
#include "DockMessage.hh"
#include "IMessageQueue.hh"
#include "LoadingTransition.hh"
#include "Services.hh"

namespace bsgalone::server {

class DockMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  DockMessageConsumer(const Services &services,
                      core::IMessageQueue *const systemMessageQueue,
                      core::IMessageQueue *const outputMessageQueue);
  ~DockMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  EntityServiceShPtr m_entityService{};
  core::IMessageQueue *const m_systemMessageQueue{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleDocking(const core::DockMessage &message) const;
  void handleUndocking(const core::DockMessage &message) const;
  void handleReturnToOutpost(const core::DockMessage &message) const;

  void publishLoadingMessages(const core::LoadingTransition transition,
                              const core::Uuid shipDbId) const;
};

} // namespace bsgalone::server
