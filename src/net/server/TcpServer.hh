
#pragma once

#include "AsioContext.hh"
#include "AsioServer.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include "INetworkServer.hh"
#include <mutex>

namespace net {

class TcpServer : public INetworkServer, public core::CoreObject
{
  public:
  TcpServer(IEventBusShPtr eventBus);
  ~TcpServer() override = default;

  void start(const int port) override;
  void stop() override;

  /// @brief - Implements the interface method to send a message to a client. Note that this
  /// function is **not thread safe**: it is assumed that the `start` method has been called
  /// prior to sending a message but it is not verified. Failing to call `start` will result
  /// in undefined behavior.
  /// @param clientId - the identifier of the client to send a message to
  /// @param bytes - the raw data to send to the client
  /// @return - an identifier for the message to allow tracking of the progress
  auto trySend(const ClientId clientId, std::vector<char> bytes)
    -> std::optional<MessageId> override;

  private:
  IEventBusShPtr m_eventBus{};

  /// @brief - Protects concurrent access to the context and server.
  std::mutex m_locker{};

  /// @brief - Whether or not the `start` method was already called. If yes, calling it
  /// again will raise an error. If a call to `stop` is made in between this value will
  /// be reset to false.
  std::atomic_bool m_started{false};

  details::AsioContextPtr m_context{};
  details::AsioServerShPtr m_server{};
};

} // namespace net
