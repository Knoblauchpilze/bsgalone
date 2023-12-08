
#include "DbConnection.hh"

namespace bsgo {
namespace {
constexpr auto DATABASE_NAME     = "bsgdb";
constexpr auto DATABASE_USER     = "bsgadmin";
constexpr auto DATABASE_HOST     = "localhost";
constexpr auto DATABASE_PORT     = "5432";
constexpr auto DATABASE_PASSWORD = "Ww76hQWbbt7zi2ItM6cNo4YYT";

/// https://www.postgresql.org/docs/current/libpq-connect.html#LIBPQ-PARAMKEYWORDS
constexpr auto DATABASE_NAME_OPTION_STRING     = "dbname";
constexpr auto DATABASE_USER_OPTION_STRING     = "user";
constexpr auto DATABASE_HOST_OPTION_STRING     = "host";
constexpr auto DATABASE_PORT_OPTION_STRING     = "port";
constexpr auto DATABASE_PASSWORD_OPTION_STRING = "password";

auto generateConnectionString() -> std::string
{
  /// https://gist.github.com/fearofcode/7516c9b7b18922386148195be5660329#file-libpqxx_test-cpp-L7
  std::string out;

  const auto addOption = [](const std::string &option, const std::string &value) {
    return option + "=" + value + " ";
  };

  out += addOption(DATABASE_NAME_OPTION_STRING, DATABASE_NAME);
  out += addOption(DATABASE_USER_OPTION_STRING, DATABASE_USER);
  out += addOption(DATABASE_HOST_OPTION_STRING, DATABASE_HOST);
  out += addOption(DATABASE_PORT_OPTION_STRING, DATABASE_PORT);
  out += addOption(DATABASE_PASSWORD_OPTION_STRING, DATABASE_PASSWORD);

  return out;
}
} // namespace

DbConnection::DbConnection()
  : utils::CoreObject("connection")
{
  setService("db");
}

void DbConnection::connect()
{
  info("Connecting to db");
  setupDbConnection();
}

void DbConnection::disconnect()
{
  info("Disconnecting from db");
  m_connection.reset();
}

auto DbConnection::connection() -> pqxx::connection &
{
  return *m_connection;
}

auto DbConnection::safeExecute(const std::string &sql) -> SqlResult
{
  return safeExecute([&sql](pqxx::connection &conn) {
    pqxx::nontransaction work(conn);
    return work.exec(sql);
  });
}

auto DbConnection::safeExecute(const SqlQuery &query) -> SqlResult
{
  SqlResult out{};

  try
  {
    out.result = query(*m_connection);
  }
  catch (const pqxx::sql_error &e)
  {
    warn("failed: " + e.query() + ", " + e.sqlstate());
    out.error = e.sqlstate();
  }

  return out;
}

void DbConnection::setupDbConnection()
{
  const auto connectionStr = generateConnectionString();
  m_connection             = std::make_unique<pqxx::connection>(connectionStr);
}

} // namespace bsgo
