
#pragma once

#include "ClientId.hh"
#include "CoreObject.hh"
#include "IMessage.hh"
#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "INetworkClient.hh"
#include "INetworkEventQueue.hh"
#include "IOutputNetworkAdapter.hh"
#include <atomic>
#include <memory>

namespace bsgalone::client {

class GameNetworkClient : public core::IMessageQueue, public ::core::CoreObject
{
  public:
  /// @brief - Creates a new client which will attempt to connect to the remote server
  /// when the start method is called.
  GameNetworkClient();

  ~GameNetworkClient() = default;

  void start(const int port);
  void stop();

  void pushEvent(core::IMessagePtr message) override;

  /// @brief - Adds a listener to the input queue held by the client. Note that
  /// any listener added through this method will persist through the start/stop
  /// cyles.
  /// The listener will be receiving messages coming from the network (i.e. the
  /// server). There's no protection against double registration.
  /// @param listener - the listener to register
  void addListener(core::IMessageListenerPtr listener) override;

  bool empty() override;

  void processEvents() override;

  private:
  net::INetworkEventQueueShPtr m_eventBus{};
  net::INetworkClientShPtr m_tcpClient{};
  core::IOutputNetworkAdapterShPtr m_adapter{};

  std::atomic_bool m_connected{};

  core::IMessageQueueShPtr m_inputQueue{};

  void initialize();
};

using GameNetworkClientShPtr = std::shared_ptr<GameNetworkClient>;

} // namespace bsgalone::client
