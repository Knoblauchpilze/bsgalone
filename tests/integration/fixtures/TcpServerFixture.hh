
#pragma once

#include "ClientId.hh"
#include "ConnectedSockets.hh"
#include "TcpAcceptor.hh"
#include "TcpFixture.hh"
#include "TestEventBus.hh"

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
  /// @return - a connected sockets struct with only the server socket populated
  auto waitForServerSocket() -> ConnectedSockets;

  /// @brief - Returns a couple of connected sockets identified as client and server:
  ///   - the client socket is triggered as part of this method
  ///   - the server socket is the one returned by the acceptor
  /// The sockets are healthy and represent the two ends of a network connection.
  /// Using this allows to test cases were it is needed to verify what happens when
  /// an operation is performed on one end of the connection.
  /// @return - two connected sockets
  auto getTestSockets() -> ConnectedSockets;

  /// @brief - This method awaits a server socket and verifies that a connection established
  /// event is received on the event bus. This means:
  ///   - waiting for a server socket
  ///   - waiting for a client established event on the test event bus
  /// The output describes the sockets received. Only the server socket holds valid data.
  /// @return - sockets received during the connection. Only the server socket will hold a
  /// valid socket
  auto waitForConnectionEstablishedEvent(TestEventBusShPtr &eventBus) -> ConnectedSockets;

  private:
  TcpAcceptorShPtr m_acceptor{};
};

} // namespace test
