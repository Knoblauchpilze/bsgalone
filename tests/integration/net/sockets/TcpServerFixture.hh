
#pragma once

#include "SocketShPtr.hh"
#include "TcpFixture.hh"
#include <gtest/gtest.h>

namespace test {

class Acceptor;

struct ConnectedSocket
{
  net::SocketShPtr client{};
  net::SocketShPtr server{};

  auto readServer(const std::size_t length) -> std::string;
  void writeServer(const std::string &data);
};

class TcpServerFixture : public TcpFixture
{
  public:
  TcpServerFixture()           = default;
  ~TcpServerFixture() override = default;

  // https://google.github.io/googletest/primer.html#same-data-multiple-tests
  protected:
  void SetUp() override;
  void TearDown() override;

  auto connectBoth() -> ConnectedSocket;

  private:
  std::thread m_contextThread{};
  std::shared_ptr<Acceptor> m_acceptor{};
};

} // namespace test
