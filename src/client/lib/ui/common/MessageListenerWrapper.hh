
#pragma once

#include "IMessageListener.hh"

namespace pge {

class MessageListenerWrapper : public bsgo::IMessageListener
{
  public:
  MessageListenerWrapper(IMessageListener *listener);
  ~MessageListenerWrapper() override = default;

  bool isMessageRelevant(const bsgo::MessageType &type) const override;
  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  IMessageListener *m_listener{};
};

} // namespace pge
