
#pragma once

#include "IGameEventListener.hh"
#include "IMessageQueue.hh"
#include "IOutputNetworkAdapter.hh"

namespace bsgalone::server {

class OutputGameEventAdapter : public IGameEventListener
{
  public:
  OutputGameEventAdapter(core::IOutputNetworkAdapterPtr networkClient);
  ~OutputGameEventAdapter() override = default;

  bool isEventRelevant(const GameEventType &type) const override;
  void onEventReceived(const IGameEvent &event) override;

  private:
  core::IOutputNetworkAdapterPtr m_networkClient{};
};

} // namespace bsgalone::server
