
#pragma once

#include "SocketShPtr.hh"
#include "TestTcpServer.hh"
#include <asio.hpp>
#include <gtest/gtest.h>
#include <thread>

namespace test {

class TcpFixture : public ::testing::Test
{
  public:
  TcpFixture();
  ~TcpFixture() override = default;

  protected:
  int m_port{};

  void SetUp() override;
  void TearDown() override;

  auto port() const -> int;

  /// @brief - Attempts to synchronously connect to the server available at
  /// the port defined in the fixture and returns the **client** socket when
  /// successful. The **server** socket is held by the TCP server and is not
  /// accessible.
  /// @return - the client socket connected to the server. The socket is in
  /// a healthy state when returned.
  auto connect() -> net::SocketShPtr;

  void write(net::SocketShPtr socket, const std::string &data);

  private:
  static std::atomic_int NEXT_PORT;

  asio::io_context m_context{};
};

} // namespace test
