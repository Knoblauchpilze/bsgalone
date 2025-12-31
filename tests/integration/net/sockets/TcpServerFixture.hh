
#pragma once

#include "SocketShPtr.hh"
#include "TestTcpServer.hh"
#include <gtest/gtest.h>

namespace test {

class TcpServerFixture : public ::testing::Test
{
  public:
  TcpServerFixture()           = default;
  ~TcpServerFixture() override = default;

  // https://google.github.io/googletest/primer.html#same-data-multiple-tests
  protected:
  void SetUp() override;
  void TearDown() override;

  auto connect() -> net::SocketShPtr;
  auto serverSocket(const std::size_t index) -> net::SocketShPtr;

  void write(const std::size_t socketIndex, const std::string &data);

  private:
  TestTcpServerShPtr m_server{};
};

} // namespace test
