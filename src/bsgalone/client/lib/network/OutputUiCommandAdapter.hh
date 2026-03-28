
#pragma once

#include "IMessageQueue.hh"
#include "IUiCommandListener.hh"

namespace bsgalone::client {

class OutputUiCommandAdapter : public IUiCommandListener
{
  public:
  OutputUiCommandAdapter(core::IMessageQueueShPtr outputQueue);
  ~OutputUiCommandAdapter() override = default;

  bool isEventRelevant(const UiCommandType &type) const override;
  void onEventReceived(const IUiCommand &event) override;

  private:
  core::IMessageQueueShPtr m_outputQueue{};
};

} // namespace bsgalone::client
