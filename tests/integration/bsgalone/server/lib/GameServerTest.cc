
#include "GameServer.hh"
#include "PortFixture.hh"
#include <future>
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::server {
using Integration_Bsgalone_Server_GameServerTest = PortFixture;

TEST_F(Integration_Bsgalone_Server_GameServerTest, StopsWhenRequested)
{
  GameServer server;

  auto result = std::async(std::launch::async, [&server]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    server.requestStop();
  });

  server.run(this->port());

  constexpr auto REASONABLE_TIMEOUT = std::chrono::milliseconds(100);
  EXPECT_EQ(std::future_status::ready, result.wait_for(REASONABLE_TIMEOUT));
}

} // namespace bsgalone::server
