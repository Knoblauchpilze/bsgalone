
#pragma once

#include "IDataStore.hh"
#include <gmock/gmock.h>

namespace test {

class MockDataStore : public bsgalone::client::IDataStore
{
  public:
  MockDataStore()           = default;
  ~MockDataStore() override = default;

  MOCK_METHOD(void,
              onPlayerLoggedIn,
              (const bsgalone::core::Uuid, const bsgalone::core::GameRole),
              (override));
};

} // namespace test
