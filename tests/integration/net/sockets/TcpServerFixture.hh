
#pragma once

#include "ConnectedSockets.hh"
#include "SocketShPtr.hh"
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

  auto getTestSockets() -> ConnectedSockets;

  private:
  TcpAcceptorShPtr m_acceptor{};
};

} // namespace test
