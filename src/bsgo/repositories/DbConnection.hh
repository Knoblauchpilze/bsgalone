
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>
#include <pqxx/pqxx>

namespace bsgo {

class DbConnection : public utils::CoreObject
{
  public:
  DbConnection();
  ~DbConnection() override = default;

  void connect();
  void disconnect();

  void prepare(const std::string &queryName, const std::string &sql);
  auto transaction() -> pqxx::work;
  auto nonTransaction() -> pqxx::nontransaction;

  auto connection() -> pqxx::connection &;

  struct SqlResult
  {
    std::optional<pqxx::result> result{};
    std::optional<std::string> error{};
  };

  auto tryExecuteQuery(const std::string &sql) -> SqlResult;

  using SqlQuery = std::function<pqxx::result(pqxx::nontransaction &)>;
  auto tryExecuteQuery(const SqlQuery &query) -> SqlResult;

  using SqlTransaction = std::function<pqxx::result(pqxx::work &)>;
  auto tryExecuteTransaction(const SqlTransaction &query) -> SqlResult;

  private:
  // https://www.tutorialspoint.com/postgresql/pdf/postgresql_c_cpp.pdf
  std::unique_ptr<pqxx::connection> m_connection{};

  void setupDbConnection();
};

using DbConnectionShPtr = std::shared_ptr<DbConnection>;

} // namespace bsgo
