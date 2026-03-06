
#include "DbConnection.hh"
#include <gtest/gtest.h>

namespace bsgalone::core {

TEST(Integration_Bsgalone_Core_Repositories_DbConnection, ConnectDisconnect)
{
  DbConnection conn;
  conn.connect();
  conn.disconnect();
}

} // namespace bsgalone::core
