
#pragma once

#include "IMessageListener.hh"

namespace bsgalone::client {

class MessageListenerWrapper : public core::IMessageListener
{
  public:
  MessageListenerWrapper(core::IMessageListener *listener);
  ~MessageListenerWrapper() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &message) override;

  private:
  core::IMessageListener *m_listener{};
};

} // namespace bsgalone::client
