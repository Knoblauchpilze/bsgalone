
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

  /// @brief - Used in the `SetUp` step to clean tables of the database
  /// which are affected by tests. This is an intermediate solution to
  /// guarantee test isolation without having to rely on test containers
  /// or recreating the database for each test.
  /// Only tables that contain user data are cleaned, not the system ones
  /// (such as system, roles, etc.).
  void cleanDatabase();
};

} // namespace test
