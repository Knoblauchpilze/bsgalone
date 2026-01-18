
#pragma once

#include "ClientId.hh"
#include "MessageId.hh"
#include <optional>
#include <vector>

namespace net {

class INetworkServer
{
  public:
  virtual ~INetworkServer() = default;

  /// @brief - Instructs the server to start listening on the specified port. This will
  /// allow remote client to connect to the server.
  /// Calling this function multiple times without calling `stop` will raise an error.
  /// @param port - the port on which to listen on
  virtual void start(const int port) = 0;

  /// @brief - Stops the server, preventing any new connection to be accepted. Existing
  /// connections will be closed gracefully.
  /// Calling this method before calling `start` will fail.
  virtual void stop() = 0;

  /// @brief - Registers data to sent to the client identified by the input data. This
  /// method will return an identifier for the message that has been sent. It allows to
  /// track the progress of the sending.
  /// This method will return an empty message identifier (indicating that the data has
  /// **not been registered** to be sent) if:
  ///   - the data is empty
  ///   - a client with the specified identifier does not exist
  /// In case the `start` method has not been called beforehand, this method will fail.
  /// @param client - the identifier of the client to send the data to
  /// @param bytes - the raw data to send to the client
  /// @return - a message identifier to use to track the sending process
  virtual auto trySend(const ClientId clientId, std::vector<char> bytes)
    -> std::optional<MessageId> = 0;
};

} // namespace net
