
#pragma once

#include "AbstractMessageConsumer.hh"
#include "DockMessage.hh"
#include "IMessageQueue.hh"
#include "LoadingTransition.hh"
#include "Services.hh"

namespace bsgo {

class DockMessageConsumer : public AbstractMessageConsumer
{
  public:
  DockMessageConsumer(const Services &services,
                      IMessageQueue *const systemMessageQueue,
                      IMessageQueue *const outputMessageQueue);
  ~DockMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};
  EntityServiceShPtr m_entityService{};
  IMessageQueue *const m_systemMessageQueue{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleDocking(const DockMessage &message) const;
  void handleUndocking(const DockMessage &message) const;
  void handleReturnToOutpost(const DockMessage &message) const;

  void publishLoadingMessages(const LoadingTransition transition,
                              const Uuid shipDbId,
                              const Uuid systemDbId,
                              const DockMessage &originalDockMessage) const;
};

} // namespace bsgo
