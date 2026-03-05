
#pragma once

#include "IMessageListener.hh"

namespace pge {

class MessageListenerWrapper : public bsgalone::core::IMessageListener
{
  public:
  MessageListenerWrapper(bsgalone::core::IMessageListener *listener);
  ~MessageListenerWrapper() override = default;

  bool isEventRelevant(const bsgalone::core::MessageType &type) const override;
  void onEventReceived(const bsgalone::core::IMessage &message) override;

  private:
  bsgalone::core::IMessageListener *m_listener{};
};

} // namespace pge
