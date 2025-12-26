
#pragma once

#include "SocketPtr.hh"
#include "TestTcpServer.hh"
#include <gtest/gtest.h>

namespace test {

class AsioNetFixture : public ::testing::Test
{
  public:
  AsioNetFixture()           = default;
  ~AsioNetFixture() override = default;

  // https://google.github.io/googletest/primer.html#same-data-multiple-tests
  protected:
  void SetUp() override;
  void TearDown() override;

  auto connect() -> net::SocketShPtr;
  auto socket(const std::size_t index) -> net::SocketShPtr;

  private:
  TestTcpServerShPtr m_server{};
};

} // namespace test
