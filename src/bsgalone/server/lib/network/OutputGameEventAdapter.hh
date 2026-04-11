
#pragma once

#include "ForManagingClient.hh"
#include "IGameEventListener.hh"
#include "IMessageQueue.hh"
#include "IOutputNetworkAdapter.hh"

namespace bsgalone::server {

class OutputGameEventAdapter : public IGameEventListener
{
  public:
  OutputGameEventAdapter(ForManagingClientShPtr clientManager,
                         core::IOutputNetworkAdapterPtr networkClient);
  ~OutputGameEventAdapter() override = default;

  bool isEventRelevant(const GameEventType &type) const override;
  void onEventReceived(const IGameEvent &event) override;

  private:
  ForManagingClientShPtr m_clientManager{};
  core::IOutputNetworkAdapterPtr m_networkClient{};
};

} // namespace bsgalone::server
