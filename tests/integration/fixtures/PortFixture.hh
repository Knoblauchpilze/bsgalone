
#pragma once

#include <atomic>
#include <gtest/gtest.h>

namespace test {

class PortFixture : public ::testing::Test
{
  public:
  PortFixture();
  ~PortFixture() override = default;

  protected:
  int m_port{};

  void SetUp() override;
  void TearDown() override;

  auto port() const -> int;

  private:
  static std::atomic_int NEXT_PORT;
};

} // namespace test
