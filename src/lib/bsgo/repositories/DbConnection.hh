
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

  private:
  std::unique_ptr<pqxx::connection> m_connection{};

  void setupDbConnection();
};

using DbConnectionShPtr = std::shared_ptr<DbConnection>;

} // namespace bsgo
