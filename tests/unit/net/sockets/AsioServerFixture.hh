
#pragma once

#include "SocketPtr.hh"
#include "TestTcpServer.hh"
#include <gtest/gtest.h>

namespace test {

class AsioServerFixture : public ::testing::Test
{
  public:
  AsioServerFixture()           = default;
  ~AsioServerFixture() override = default;

  // https://google.github.io/googletest/primer.html#same-data-multiple-tests
  protected:
  void SetUp() override;
  void TearDown() override;

  auto connect() -> net::SocketShPtr;
  auto socket(const std::size_t index) -> net::SocketShPtr;
  auto port() const -> int;

  void writeTo(const std::size_t index, const std::string &data);

  void waitForABit();

  private:
  TestTcpServerShPtr m_server{};
};

} // namespace test
