
#pragma once

#include "CoreObject.hh"
#include "ForManagingClient.hh"
#include "IGameEventListener.hh"
#include "IMessageQueue.hh"
#include "IOutputNetworkAdapter.hh"
#include "PlayerLogoutEvent.hh"

namespace bsgalone::server {

class OutputGameEventAdapter : public IGameEventListener, public ::core::CoreObject
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

  void handleLogoutEvent(const PlayerLogoutEvent &event);
};

} // namespace bsgalone::server
