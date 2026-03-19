
#pragma once

#include "IMessageQueue.hh"
#include "IUiEvent.hh"
#include "IUiEventListener.hh"
#include "IUiEventQueue.hh"
#include "LoginCommand.hh"

namespace bsgalone::client {

class OutputUiCommandAdapter : public IUiEventListener
{
  public:
  OutputUiCommandAdapter(core::IMessageQueueShPtr outputQueue);
  ~OutputUiCommandAdapter() override = default;

  bool isEventRelevant(const UiEventType &type) const override;
  void onEventReceived(const IUiEvent &event) override;

  private:
  core::IMessageQueueShPtr m_outputQueue{};

  void publishLoginMessage(const LoginCommand &command);
};

} // namespace bsgalone::client
