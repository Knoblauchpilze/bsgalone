
#include "AsioContext.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace net::details {

TEST(Unit_Net_Server_AsioContext, CorrectlyResolvesLocalhost)
{
  AsioContext context{};

  const auto endpoints = context.resolve("127.0.0.1", 3564);

  EXPECT_EQ(1, endpoints.size());
  EXPECT_EQ("127.0.0.1", endpoints->host_name());
  EXPECT_EQ("3564", endpoints->service_name());
}

} // namespace net::details
