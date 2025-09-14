
#include "DbConnection.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include <gtest/gtest.h>

namespace bsgo {

constexpr auto TEST_QUERY_NAME = "test-query";
constexpr auto TEST_QUERY      = R"(
SELECT
  created_at
FROM
  player
WHERE
  id = $1
)";

TEST(Unit_Bsgo_Repositories_DatabaseTime, ReadFromDatabase)
{
  DbConnection conn;
  conn.connect();
  conn.prepare(TEST_QUERY_NAME, TEST_QUERY);

  const auto query = [](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{TEST_QUERY_NAME}, pqxx::params{toDbId(0)}).one_row();
  };
  const auto record = conn.executeQueryReturningSingleRow(query);

  const auto actual = record[0].as<std::string>();

  EXPECT_EQ("hehe", actual);
}

TEST(Unit_Bsgo_Repositories_DatabaseTime, ParseFromDatabase)
{
  constexpr auto utcTimeStr = "2025-09-12 15:22:54.057426+00";

  const auto actual = std::chrono::utc_clock::from_stream(utcTimeStr);
  EXPECT_EQ("hehe", actual);
}

} // namespace bsgo
