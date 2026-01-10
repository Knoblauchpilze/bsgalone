
#pragma once

#include "ConnectedSockets.hh"
#include "TcpAcceptor.hh"
#include "TcpFixture.hh"

namespace test {

class TcpServerFixture : public TcpFixture
{
  public:
  TcpServerFixture()           = default;
  ~TcpServerFixture() override = default;

  // https://google.github.io/googletest/primer.html#same-data-multiple-tests
  protected:
  void SetUp() override;
  void TearDown() override;

  /// @brief - Waits for the acceptor to detect an incoming connection and signal it as
  /// a socket. This function can be called after an asynchronous connect request has
  /// been made against the acceptor.
  /// If no connection request is pending, the method will throw.
  /// @return - the server socket received while waiting
  auto waitForServerSocket() -> net::details::SocketShPtr;

  /// @brief - Returns a couple of connected sockets identified as client and server:
  ///   - the client socket is triggered as part of this method
  ///   - the server socket is the one returned by the acceptor
  /// The sockets are healthy and represent the two ends of a network connection.
  /// Using this allows to test cases were it is needed to verify what happens when
  /// an operation is performed on one end of the connection.
  /// @return - two connected sockets
  auto getTestSockets() -> ConnectedSockets;

  private:
  TcpAcceptorShPtr m_acceptor{};
};

} // namespace test
