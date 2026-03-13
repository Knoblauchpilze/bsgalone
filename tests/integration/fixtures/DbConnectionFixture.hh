
#pragma once

#include "DbConnection.hh"
#include <gtest/gtest.h>

namespace test {

class DbConnectionFixture : public ::testing::Test
{
  public:
  DbConnectionFixture();
  ~DbConnectionFixture() override = default;

  protected:
  void SetUp() override;
  void TearDown() override;

  auto dbConnection() const -> bsgalone::core::DbConnectionShPtr;

  private:
  bsgalone::core::DbConnectionShPtr m_dbConn{};
};

} // namespace test
