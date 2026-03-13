
#include "DbConnectionFixture.hh"

namespace test {

DbConnectionFixture::DbConnectionFixture()
  : m_dbConn(std::make_shared<bsgalone::core::DbConnection>())
{}

void DbConnectionFixture::SetUp()
{
  m_dbConn->connect();
}

void DbConnectionFixture::TearDown()
{
  m_dbConn->disconnect();
}

auto DbConnectionFixture::dbConnection() const -> bsgalone::core::DbConnectionShPtr
{
  return m_dbConn;
}

} // namespace test
