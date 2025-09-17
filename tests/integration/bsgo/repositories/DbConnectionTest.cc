
#include "DbConnection.hh"
#include <gtest/gtest.h>

namespace bsgo {

TEST(Integration_Bsgo_DbConnection, ConnectDisconnect)
{
  DbConnection conn;
  conn.connect();
  conn.disconnect();
}

} // namespace bsgo
