
#pragma once

#include "IDataStore.hh"
#include "IMessageListener.hh"
#include "IUiEventQueue.hh"

namespace bsgalone::client {

class LoginMessageConsumer : public core::IMessageListener
{
  public:
  /// @brief - Creates a new message consumer responsible to process login
  /// messages. Upon receiving a message, this consumer will inform the
  /// data store if the login succeeded and publish the corresponding events
  /// to the queue so that the UI is informed.
  /// @param store - the data store when login information should be stored
  /// @param queue - a queue to publish UI relevant updates
  LoginMessageConsumer(IDataStoreShPtr store, IUiEventQueueShPtr queue);
  ~LoginMessageConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &event) override;

  private:
  IDataStoreShPtr m_store{};
  IUiEventQueueShPtr m_queue{};
};

} // namespace bsgalone::client
