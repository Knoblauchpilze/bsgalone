
#pragma once

#include "CoreObject.hh"
#include <memory>
#include <pqxx/pqxx>

namespace bsgalone::core {

class DbConnection : public ::core::CoreObject
{
  public:
  DbConnection();
  ~DbConnection() override = default;

  /// @brief - Attempts to connect to the remote database. May fail in case the
  /// remote database is not reachable in which case the underlying library used
  /// to represent the connection might throw.
  /// Note: this function is **not thread safe** and is **not guarded against
  /// being called multiple times**. Doing so is undefined behavior.
  void connect();

  /// @brief - Disconnects from the remote database. After calling this function
  /// any call to `executeQuery` and other functions will fail. Calling `connect`
  /// again will allow to execute new queries again.
  /// Note: this function is **not thread safe** but can be called multiple times.
  void disconnect();

  /// @brief - PRepares a SQL query for future use. This approach is recommended for
  /// queries that can be used multiple times as it improves performance.
  /// @param queryName - a unique name to refer to the query after it's been prepared
  /// @param sql - the SQL query
  void prepare(const std::string &queryName, const std::string &sql);

  /// @brief - An alias for a function using a non transaction to return a list
  /// of rows as a result.
  using SqlQuery = std::function<pqxx::result(pqxx::nontransaction &)>;

  /// @brief - Execute the input query and assert that no error occurs. In case
  /// we face a sql error this method will throw an exception. The caller can be
  /// guaranteed that the result is valid if the method returns.
  /// Will throw in case `connect` has not been called.
  /// @param query - the query to execute
  /// @return - the return of the query.
  auto executeQuery(const SqlQuery &query) -> pqxx::result;

  /// @brief - An alias for a function using a non transaction to return a single
  /// of rows as a result.
  using SqlQueryReturningSingleRow = std::function<pqxx::row(pqxx::nontransaction &)>;

  /// @brief - Execute the input query and assert that no error occurs. The query
  /// is expected to return a single row when successful.
  /// Will throw in case `connect` has not been called.
  /// @param query - the query to execute.
  /// @return - the row returned by the query.
  auto executeQueryReturningSingleRow(const SqlQueryReturningSingleRow &query) -> pqxx::row;

  /// @brief - An alias for a function using a transaction to return a list of rows
  /// as a result.
  using SqlTransaction = std::function<pqxx::result(pqxx::work &)>;

  struct SqlResult
  {
    std::optional<pqxx::result> result{};
    std::optional<std::string> error{};
  };

  /// @brief - Executes the query in a transaction where all statements will
  /// be applied if and only if all of them succeed. No changes will be applied
  /// in case one of the statement raises an error.
  /// Will throw in case `connect` has not been called.
  /// @param query - the query to execute
  /// @return - a result with information about potential errors or a result.
  auto tryExecuteTransaction(const SqlTransaction &query) -> SqlResult;

  private:
  // https://www.tutorialspoint.com/postgresql/pdf/postgresql_c_cpp.pdf
  std::unique_ptr<pqxx::connection> m_connection{};

  void setupDbConnection();
  void checkConnected();
  auto transaction() -> pqxx::work;
  auto nonTransaction() -> pqxx::nontransaction;
};

using DbConnectionShPtr = std::shared_ptr<DbConnection>;

} // namespace bsgalone::core
