
#pragma once

#include "AbstractMessageListener.hh"
#include "IUiEvent.hh"
#include "IUiEventQueue.hh"

namespace bsgalone::client {

class InputGameMessageAdapter : public core::AbstractMessageListener
{
  public:
  InputGameMessageAdapter(IUiEventQueueWPtr uiQueue);
  ~InputGameMessageAdapter() override = default;

  void onEventReceived(const core::IMessage &event) override;

  private:
  IUiEventQueueWPtr m_uiQueue{};

  void tryPushUiEvent(const IUiEvent &event);
};

} // namespace bsgalone::client
