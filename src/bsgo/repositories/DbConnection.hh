
#pragma once

#include "CoreObject.hh"
#include <memory>
#include <pqxx/pqxx>

namespace bsgo {

class DbConnection : public core::CoreObject
{
  public:
  DbConnection();
  ~DbConnection() override = default;

  void connect();
  void disconnect();

  void prepare(const std::string &queryName, const std::string &sql);

  /// @brief - An alias for a function using a non transaction to return a list
  /// of rows as a result.
  using SqlQuery = std::function<pqxx::result(pqxx::nontransaction &)>;

  /// @brief - Execute the input query and assert that no error occurs. In case
  /// we face a sql error this method will throw an exception. The caller can be
  /// guaranteed that the result is valid if the method returns.
  /// @param query - the query to execute
  /// @return - the return of the query.
  auto executeQuery(const SqlQuery &query) -> pqxx::result;

  struct SqlResult
  {
    std::optional<pqxx::result> result{};
    std::optional<std::string> error{};
  };

  /// @brief - Attempt to execute the input query and return the potential result
  /// or any error that occurred during the execution. The caller should always
  /// check first for the error and if nothing is reported the result can be used.
  /// @param query - the query to execute.
  /// @return - the execution result composed of a potential error and result.
  auto tryExecuteQuery(const SqlQuery &query) -> SqlResult;

  /// @brief - An alias for a function using a non transaction to return a single
  /// of rows as a result.
  using SqlQueryReturningSingleRow = std::function<pqxx::row(pqxx::nontransaction &)>;

  /// @brief - Execute the input query and assert that no error occurs. The query
  /// is expected to return a single row when successful.
  /// @param query - the query to execute.
  /// @return - the row returned by the query.
  auto executeQueryReturningSingleRow(const SqlQueryReturningSingleRow &query) -> pqxx::row;

  struct SingleSqlRowResult
  {
    std::optional<pqxx::row> result{};
    std::optional<std::string> error{};
  };

  /// @brief - Attempt to execute the input query and return the potential result
  /// expected to be composed of a single row when successful. In case the query
  /// failes the return value will include this error and no result.
  /// The caller should always check first for the error and if nothing is reported
  /// the result can be used.
  /// @param query - the query to execute.
  /// @return - the execution result composed of a potential error and a single row
  /// result.
  auto tryExecuteQueryReturningSingleRow(const SqlQueryReturningSingleRow &query)
    -> SingleSqlRowResult;

  /// @brief - An alias for a function using a transaction to return a list of rows
  /// as a result.
  using SqlTransaction = std::function<pqxx::result(pqxx::work &)>;

  auto tryExecuteTransaction(const SqlTransaction &query) -> SqlResult;

  private:
  // https://www.tutorialspoint.com/postgresql/pdf/postgresql_c_cpp.pdf
  std::unique_ptr<pqxx::connection> m_connection{};

  void setupDbConnection();
  auto transaction() -> pqxx::work;
  auto nonTransaction() -> pqxx::nontransaction;
};

using DbConnectionShPtr = std::shared_ptr<DbConnection>;

} // namespace bsgo
