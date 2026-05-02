
#include "DbQueryHelper.hh"
#include "TimeUtils.hh"

namespace test {

auto insertTestAccount(bsgalone::core::DbConnection &dbConnection) -> bsgalone::server::Account
{
  const bsgalone::core::Uuid uuid;
  const auto username = std::format("random-account-{:%F%T}", core::now());

  constexpr auto QUERY = R"(
      INSERT INTO account ("id", "name", "password")
        VALUES ($1, $2, 'password')
    )";

  const auto query = [&uuid, &username](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{uuid.toDbId(), username});
  };
  dbConnection.executeQuery(query);

  return bsgalone::server::Account{
    .dbId     = uuid,
    .username = username,
    .password = "password",
  };
}

auto insertTestSystem(bsgalone::core::DbConnection &dbConnection) -> bsgalone::server::System
{
  const bsgalone::core::Uuid uuid;
  // https://stackoverflow.com/questions/34857119/how-to-convert-stdchronotime-point-to-string
  // https://en.cppreference.com/w/cpp/chrono/system_clock/formatter.html
  const auto name = std::format("random-system-{:%F%T}", core::now());

  constexpr auto QUERY = R"(
      INSERT INTO system ("id", "name", "x_pos", "y_pos", "z_pos")
        VALUES ($1, $2, 2.5, -1.2, 3.4)
    )";

  const auto query = [&uuid, &name](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{uuid.toDbId(), name});
  };
  dbConnection.executeQuery(query);

  return bsgalone::server::System{
    .dbId     = uuid,
    .name     = name,
    .position = Eigen::Vector3f(2.5f, -1.2f, 3.4f),
  };
}

} // namespace test
