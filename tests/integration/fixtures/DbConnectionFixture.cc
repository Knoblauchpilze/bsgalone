
#include "DbConnectionFixture.hh"

namespace test {

DbConnectionFixture::DbConnectionFixture()
  : m_dbConn(std::make_shared<bsgalone::core::DbConnection>())
{}

void DbConnectionFixture::SetUp()
{
  m_dbConn->connect();
  cleanDatabase();
}

void DbConnectionFixture::TearDown()
{
  m_dbConn->disconnect();
}

auto DbConnectionFixture::dbConnection() const -> bsgalone::core::DbConnectionShPtr
{
  return m_dbConn;
}

namespace {
// Note: the sequences are not reset: this is because this would require
// executing the tests with the admin user as sequences belong to the user
// who created them. As tests do not depend on precise identifiers it is
// not an issue.
constexpr auto RESET_DATABASE_QUERY = R"(
TRUNCATE TABLE
  ai_behaviors,
  ai_targets,
  player_ship_respawn,
  asteroid_respawn,
  ship_jump,
  ship_system,
  ship_computer,
  ship_weapon,
  player_role,
  player_ship,
  player_computer,
  player_weapon,
  player_resource,
  player,
  account
)";
} // namespace

void DbConnectionFixture::cleanDatabase()
{
  const auto query = [](pqxx::nontransaction &work) {
    return work.exec(RESET_DATABASE_QUERY).no_rows();
  };

  m_dbConn->executeQuery(query);
}

} // namespace test
