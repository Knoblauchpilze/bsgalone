
#pragma once

#include "ForManagingClient.hh"
#include <gmock/gmock.h>

namespace test {

class MockClientManager : public bsgalone::server::ForManagingClient
{
  public:
  MockClientManager()           = default;
  ~MockClientManager() override = default;

  MOCK_METHOD(void,
              registerPlayer,
              (const net::ClientId, const bsgalone::core::Uuid, const bsgalone::core::Uuid),
              (override));
};

} // namespace test
