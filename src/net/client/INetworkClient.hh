
#pragma once

#include "MessageId.hh"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace net {

class INetworkClient
{
  public:
  virtual ~INetworkClient() = default;

  /// @brief - Instructs the client to asynchronously attempt to connect to the remote
  /// endpoint defined by `url` and `port`. This function will return immediately and
  /// will not wait for the connection attempt to either succeed or fail.
  /// @param url - the URL of the server to connect to
  /// @param port - the port on which the server is listening on
  virtual void connect(const std::string &url, const int port) = 0;

  /// @brief - Synchronously disconnect the client from the remote endpoint.
  /// This method will block until the client is disconnected.
  /// Calling this method before calling `connect` will fail.
  virtual void disconnect() = 0;

  /// @brief - Registers data to sent to the remote endpoint this client is connected to.
  /// In case the `connect` method has not been called and a connection has been established
  /// successfully, this method will raise an error.
  /// @param bytes - the raw data to send to the remote endpoint
  /// @return - a message identifier to use to track the sending process
  virtual auto trySend(std::vector<char> bytes) -> std::optional<MessageId> = 0;
};

using INetworkClientPtr = std::unique_ptr<INetworkClient>;

} // namespace net
