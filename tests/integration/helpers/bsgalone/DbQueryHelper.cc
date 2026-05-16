
#include "DbQueryHelper.hh"
#include "TimeUtils.hh"

namespace test {

auto insertTestAccount(bsgalone::server::DbConnection &dbConnection) -> bsgalone::server::Account
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

auto insertTestSystem(bsgalone::server::DbConnection &dbConnection) -> bsgalone::server::System
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

namespace {
auto determineShipId(const bsgalone::core::Faction faction) -> bsgalone::core::Uuid
{
  // The ship is hardcoded to:
  //  - Viper Mark II for the colonial faction
  //  - Cylon Raider for the cylon faction
  switch (faction)
  {
    case bsgalone::core::Faction::COLONIAL:
      return bsgalone::core::Uuid::fromDbId("e890bb9e-91f9-434f-a6c5-2228b00336b7");
    case bsgalone::core::Faction::CYLON:
      return bsgalone::core::Uuid::fromDbId("2f0c6866-b39c-44d3-bf1c-050de7984778");
    default:
      throw std::invalid_argument("Unsupported faction " + bsgalone::core::str(faction));
  }
}
} // namespace

void insertTestShip(bsgalone::server::DbConnection &dbConnection,
                    const bsgalone::core::Uuid playerDbId,
                    const bsgalone::core::Faction faction,
                    const bool active,
                    const std::optional<bsgalone::core::Uuid> &maybeSystem)
{
  const bsgalone::core::Uuid uuid;
  const auto shipId = determineShipId(faction);

  constexpr auto QUERY = R"(
      INSERT INTO player_ship ("id", "ship", "player", "name", "active", "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
        VALUES ($1, $2, $3, 'sample-ship', $4, 51.17, 32.26, 2.5, -1.2, 3.4)
    )";

  const auto query = [&uuid, &shipId, &playerDbId, active](pqxx::nontransaction &work) {
    return work.exec(QUERY,
                     pqxx::params{uuid.toDbId(), shipId.toDbId(), playerDbId.toDbId(), active});
  };
  dbConnection.executeQuery(query);

  if (maybeSystem)
  {
    constexpr auto SYSTEM_QUERY = R"(
      INSERT INTO ship_system ("ship", "system", "docked")
        VALUES ($1, $2, true)
    )";

    const auto query = [&uuid, &maybeSystem](pqxx::nontransaction &work) {
      return work.exec(SYSTEM_QUERY, pqxx::params{uuid.toDbId(), maybeSystem->toDbId()});
    };
    dbConnection.executeQuery(query);
  }
}

} // namespace test
