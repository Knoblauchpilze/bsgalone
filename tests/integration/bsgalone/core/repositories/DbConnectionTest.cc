
#include "DbConnection.hh"
#include <gtest/gtest.h>

namespace bsgo {

TEST(Integration_Bsgalone_Core_Repositories_DbConnection, ConnectDisconnect)
{
  DbConnection conn;
  conn.connect();
  conn.disconnect();
}

} // namespace bsgo
