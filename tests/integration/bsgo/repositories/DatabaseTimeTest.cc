
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

TEST(Integration_Bsgo_DatabaseTime, ReadFromDatabase)
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

} // namespace bsgo
