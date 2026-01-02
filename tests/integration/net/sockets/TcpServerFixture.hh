
#pragma once

#include "ConnectedSockets.hh"
#include "SocketShPtr.hh"
#include "TcpAcceptor.hh"
#include "TcpFixture.hh"
#include <gtest/gtest.h>

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

  auto connectBoth() -> ConnectedSockets;

  private:
  std::thread m_contextThread{};
  TcpAcceptorShPtr m_acceptor{};
};

} // namespace test
