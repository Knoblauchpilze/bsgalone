
#pragma once

#include "ClientId.hh"
#include "IMessage.hh"
#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "INetworkClient.hh"
#include "INetworkEventQueue.hh"
#include "IOutputNetworkAdapter.hh"
#include "ServerConfig.hh"
#include <atomic>
#include <memory>

namespace bsgalone::client {

class GameNetworkClient : public core::IMessageQueue
{
  public:
  GameNetworkClient();
  ~GameNetworkClient() = default;

  void setAutoLogin(User autoLogin);

  void start(const int port);
  void stop();

  void pushEvent(core::IMessagePtr message) override;
  void addListener(core::IMessageListenerPtr listener) override;
  bool empty() override;

  void processEvents() override;

  private:
  net::INetworkEventQueueShPtr m_eventBus{};
  net::INetworkClientShPtr m_tcpClient{};
  core::IOutputNetworkAdapterShPtr m_adapter{};

  std::atomic_bool m_connected{};

  core::IMessageQueueShPtr m_inputQueue{};

  /// @brief - When set, represents the credentials to use to register an
  /// automatic login attempt to the server. This is mainly used during
  /// the development process to allow faster login.
  std::optional<User> m_autoLogin{};

  void initialize();
};

using GameNetworkClientShPtr = std::shared_ptr<GameNetworkClient>;

} // namespace bsgalone::client
