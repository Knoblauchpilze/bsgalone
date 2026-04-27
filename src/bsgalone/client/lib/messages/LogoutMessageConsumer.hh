
#pragma once

#include "IDataStore.hh"
#include "IMessageListener.hh"
#include "IUiEventQueue.hh"

namespace bsgalone::client {

class LogoutMessageConsumer : public core::IMessageListener
{
  public:
  /// @brief - Creates a new message consumer responsible to process logout
  /// messages. Upon receiving a message, this consumer will inform the
  /// data store that the player is no longer logged in and publish the
  /// corresponding events to the queue so that the UI is informed.
  /// In case the message is received for a different player than the one
  /// currently logged in, no UI message will be puslibhed.
  /// @param store - the data store where logout information should be stored
  /// @param queue - a queue to publish UI relevant updates
  LogoutMessageConsumer(IDataStoreShPtr store, IUiEventQueueShPtr queue);
  ~LogoutMessageConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &event) override;

  private:
  IDataStoreShPtr m_store{};
  IUiEventQueueShPtr m_queue{};
};

} // namespace bsgalone::client
