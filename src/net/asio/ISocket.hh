
#pragma once

#include <string>
#include <vector>

namespace net {

class ISocket
{
  public:
  virtual ~ISocket() = default;

  /// @brief - Returns a human readable string to represent the endpoint this
  /// socket is connected to.
  /// It should look like the following: `"127.0.0.1:56726"`.
  /// @return - a string representing the endpoint this socket is connected to.
  virtual auto endpoint() const -> std::string = 0;

  /// @brief - Returns true if the socket is still connected to the remote
  /// endpoint. This can be used to determine wteher the socke is still healthy
  /// (meaning it can receive and send data) or not.
  /// @return - whether the socket is still connected to the remote host.
  virtual bool isConnected() const = 0;

  /// @brief - Closes the termination of the connection with the remote host.
  /// This operation is idempotent. After calling this method it is expected
  /// that:
  ///  - data cannot be received from the remote host anymore
  ///  - data cannot be transmitted to the remote host
  ///  - any outstanding read/write operation might be left in an incomplete
  ///    state (e.g. receive incomplete data)
  virtual void close() = 0;

  /// @brief - Instructs the socket to connect to the remote host. This operation
  /// is asynchronous and does not return any usable code. For cases when it is
  /// necessary to know whether a connection is active, it is possible to poll
  /// the `isConnected` method.
  virtual void connect() = 0;

  /// @brief - Instructs the socket to transmit the data described by `bytes` to
  /// the remote host. It is necessary to have called `connect` prior to calling
  /// the send method. Failure to do so is unefined behavior.
  /// The send operation is asynchronous: this means that there's no way to know
  /// when the data was actually sent.
  /// @param bytes - the data to transmit
  virtual void send(const std::vector<char> &bytes) = 0;

  /// @brief - Returns the data read from the remote host. This operation is not
  /// polling data directly from the host: instead, it returns any pending data
  /// that was received asynchronously from the host.
  /// Some notes:
  ///   - once the data is returned, it is discarded from the internal buffers,
  ///     meaning that it is not possible to retrive it again. Calling code should
  ///     handle storing it in case it is incomplete
  ///   - in case nothing has been received, an empty vector will be returned
  /// The calling code should call `connect` before attempting to read data as
  /// data can only be received after connecting to the remote host.
  /// @return - the data received from the host since the last call to `read`
  virtual auto read() const -> std::vector<char> = 0;
};

} // namespace net
