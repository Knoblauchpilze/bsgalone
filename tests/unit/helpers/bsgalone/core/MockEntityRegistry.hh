
#pragma once

#include "ForRegisteringEntity.hh"
#include <gmock/gmock.h>

namespace test {

class MockEntityRegistry : public bsgalone::core::ForRegisteringEntity
{
  public:
  MockEntityRegistry()           = default;
  ~MockEntityRegistry() override = default;

  MOCK_METHOD(void, registerAsteroid, (const bsgalone::core::Asteroid &), (override));
};

} // namespace test
