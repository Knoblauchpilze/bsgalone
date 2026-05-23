
#pragma once

#include "IDataStore.hh"
#include "IGame.hh"
#include "IMessageListener.hh"
#include "IUiEventQueue.hh"

namespace bsgalone::client {

class SystemDataMessageConsumer : public core::IMessageListener
{
  public:
  SystemDataMessageConsumer(IGameShPtr game, IUiEventQueueShPtr queue);
  ~SystemDataMessageConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &event) override;

  private:
  IGameShPtr m_game{};
  IUiEventQueueShPtr m_queue{};
};

} // namespace bsgalone::client
