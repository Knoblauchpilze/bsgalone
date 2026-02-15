
#pragma once

#include "IMessageListener.hh"

namespace pge {

class MessageListenerWrapper : public bsgalone::core::IMessageListener
{
  public:
  MessageListenerWrapper(bsgalone::core::IMessageListener *listener);
  ~MessageListenerWrapper() override = default;

  bool isMessageRelevant(const bsgo::MessageType &type) const override;
  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  bsgalone::core::IMessageListener *m_listener{};
};

} // namespace pge
