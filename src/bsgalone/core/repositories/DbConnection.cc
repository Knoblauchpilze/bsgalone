
#include "DbConnection.hh"
#include "Environment.hh"

namespace bsgo {
namespace {
constexpr auto DATABASE_NAME     = "db_bsgalone";
constexpr auto DATABASE_USER     = "bsgalone_manager";
constexpr auto ENV_DATABASE_HOST = "DB_HOST";
constexpr auto DATABASE_HOST     = "localhost";
constexpr auto DATABASE_PORT     = "5432";
constexpr auto DATABASE_PASSWORD = "manager_password";

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

  const auto host = core::getEnvironmentVariable<std::string>(ENV_DATABASE_HOST, DATABASE_HOST);
  out += addOption(DATABASE_HOST_OPTION_STRING, host);

  out += addOption(DATABASE_PORT_OPTION_STRING, DATABASE_PORT);
  out += addOption(DATABASE_PASSWORD_OPTION_STRING, DATABASE_PASSWORD);

  return out;
}
} // namespace

DbConnection::DbConnection()
  : core::CoreObject("connection")
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

void DbConnection::prepare(const std::string &queryName, const std::string &sql)
{
  m_connection->prepare(queryName, sql);
}

auto DbConnection::executeQuery(const SqlQuery &query) -> pqxx::result
{
  const auto res = tryExecuteQuery(query);
  if (res.error)
  {
    error("Failed to execute query", *res.error);
  }

  return *res.result;
}

auto DbConnection::tryExecuteQuery(const SqlQuery &query) -> SqlResult
{
  SqlResult out{};

  try
  {
    auto work  = nonTransaction();
    out.result = query(work);
  }
  catch (const pqxx::sql_error &e)
  {
    warn("Failed to execute sql query",
         "Query: " + e.query() + ", code: " + e.sqlstate() + ", what: " + e.what()
           + ", location[column: " + std::to_string(e.location.column()) + ", file_name:"
           + e.location.file_name() + ", function_name: " + e.location.function_name()
           + ", line: " + std::to_string(e.location.line()));
    out.error = e.sqlstate();
  }
  catch (const std::exception &e)
  {
    warn("Failed to execute sql query", e.what());
    out.error = e.what();
  }

  return out;
}

auto DbConnection::executeQueryReturningSingleRow(const SqlQueryReturningSingleRow &query)
  -> pqxx::row
{
  const auto res = tryExecuteQueryReturningSingleRow(query);
  if (res.error)
  {
    error("Failed to execute query returning single row", *res.error);
  }

  return *res.result;
}

auto DbConnection::tryExecuteQueryReturningSingleRow(const SqlQueryReturningSingleRow &query)
  -> SingleSqlRowResult
{
  SingleSqlRowResult out{};

  try
  {
    auto work  = nonTransaction();
    out.result = query(work);
  }
  catch (const pqxx::sql_error &e)
  {
    warn("Failed to execute sql query returning single row",
         "Query: " + e.query() + ", code: " + e.sqlstate());
    out.error = e.sqlstate();
  }
  catch (const std::exception &e)
  {
    warn("Failed to execute sql query returning single row", e.what());
    out.error = e.what();
  }

  return out;
}

auto DbConnection::tryExecuteTransaction(const SqlTransaction &query) -> SqlResult
{
  SqlResult out{};

  try
  {
    auto work  = transaction();
    out.result = query(work);
    work.commit();
  }
  catch (const pqxx::sql_error &e)
  {
    warn("Failed to execute transaction", "Query: " + e.query() + ", code: " + e.sqlstate());
    out.error = e.sqlstate();
  }
  catch (const std::exception &e)
  {
    warn("Failed to execute transaction", e.what());
    out.error = e.what();
  }

  return out;
}

void DbConnection::setupDbConnection()
{
  const auto connectionStr = generateConnectionString();
  m_connection             = std::make_unique<pqxx::connection>(connectionStr);
}

auto DbConnection::transaction() -> pqxx::work
{
  return pqxx::work(*m_connection);
}

auto DbConnection::nonTransaction() -> pqxx::nontransaction
{
  return pqxx::nontransaction(*m_connection);
}

} // namespace bsgo
