
#pragma once

#include "IMessageListener.hh"
#include "IUiEventQueue.hh"

namespace bsgalone::client {

class SignupMessageConsumer : public core::IMessageListener
{
  public:
  explicit SignupMessageConsumer(IUiEventQueueShPtr queue);
  ~SignupMessageConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &event) override;

  private:
  IUiEventQueueShPtr m_queue{};
};

} // namespace bsgalone::client
