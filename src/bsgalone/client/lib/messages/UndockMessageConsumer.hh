
#pragma once

#include "IDataStore.hh"
#include "IMessageListener.hh"
#include "IUiEventQueue.hh"

namespace bsgalone::client {

class UndockMessageConsumer : public core::IMessageListener
{
  public:
  UndockMessageConsumer(IDataStoreShPtr store, IUiEventQueueShPtr queue);
  ~UndockMessageConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &event) override;

  private:
  IDataStoreShPtr m_store{};
  IUiEventQueueShPtr m_queue{};
};

} // namespace bsgalone::client
