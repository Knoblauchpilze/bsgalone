
#pragma once

#include "AsioContext.hh"
#include "SocketShPtr.hh"
#include <asio.hpp>
#include <atomic>
#include <gtest/gtest.h>

namespace test {

class TcpFixture : public ::testing::Test
{
  public:
  TcpFixture();
  ~TcpFixture() override = default;

  protected:
  int m_port{};
  net::details::AsioContextPtr m_context{std::make_unique<net::details::AsioContext>()};

  void SetUp() override;
  void TearDown() override;

  auto port() const -> int;
  auto asioContext() -> net::details::AsioContext &;

  /// @brief - Attempts to synchronously connect to the server available at
  /// the port defined in the fixture and returns the **client** socket when
  /// successful. The **server** socket is held by the TCP server and is not
  /// accessible.
  /// Note: the server should **already be running**, otherwise this method
  /// will hang.
  /// @return - the client socket connected to the server. The socket is in
  /// a healthy state when returned.
  auto connectToRunningServer() -> net::details::SocketShPtr;

  void write(net::details::SocketShPtr socket, const std::string &data);

  private:
  static std::atomic_int NEXT_PORT;
};

} // namespace test
