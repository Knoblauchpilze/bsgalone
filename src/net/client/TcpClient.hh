
#pragma once

#include "AsioClient.hh"
#include "AsioContext.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include "INetworkClient.hh"
#include <mutex>

namespace net {

class TcpClient : public INetworkClient, public core::CoreObject
{
  public:
  // TODO: Maybe this could be returned from a `getEventBus` method
  // or provided as an API similar to `addListener`
  TcpClient(IEventBusShPtr eventBus);
  ~TcpClient() override = default;

  /// @brief - Implements the client interface to asynchronously connect to the server
  /// listening at the specified port.
  /// When the connection is established, an event will be puslihed in the event bus
  /// provided as argument during the construction. If the connection fails, nothing
  /// will be provided for now. This can be changed if needed in the future.
  /// @param url - the URL to connect to
  /// @param port - the port to connect to
  void connect(const std::string &url, const int port) override;

  /// @brief - Synchronously disconnect the client from the remote endpoint.
  /// This method will block until the client is disconnected.
  /// Calling this method before calling `connect` will fail.
  void disconnect() override;

  /// @brief - Implements the interface method to send a message to the remote server.
  /// Note that this function is **not thread safe**: it is assumed that the `start`
  /// method has been called prior to sending a message but it is not verified. Failing
  /// to call `start` will result in undefined behavior.
  /// The message will be associated with the client identifier received from the
  /// server when establishing the connection.
  /// @param bytes - the raw data to send to the client
  /// @return - an identifier for the message to allow tracking of the progress
  auto trySend(std::vector<char> bytes) -> std::optional<MessageId> override;

  private:
  IEventBusShPtr m_eventBus{};

  /// @brief - Protects concurrent access to the context and client.
  std::mutex m_locker{};

  /// @brief - Whether or not the `start` method was already called. If yes, calling it
  /// again will raise an error. If a call to `stop` is made in between this value will
  /// be reset to false.
  std::atomic_bool m_started{false};

  details::AsioContextPtr m_context{};
  details::AsioClientShPtr m_client{};
};

} // namespace net
