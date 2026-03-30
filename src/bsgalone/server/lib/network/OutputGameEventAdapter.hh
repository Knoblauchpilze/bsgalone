
#pragma once

#include "IGameEventListener.hh"
#include "IMessageQueue.hh"

namespace bsgalone::server {

class OutputGameEventAdapter : public IGameEventListener
{
  public:
  OutputGameEventAdapter(core::IMessageQueueShPtr outputQueue);
  ~OutputGameEventAdapter() override = default;

  bool isEventRelevant(const core::GameEventType &type) const override;
  void onEventReceived(const core::IGameEvent &event) override;

  private:
  core::IMessageQueueShPtr m_outputQueue{};
};

} // namespace bsgalone::server
