
#pragma once

#include "ForRegisteringEntity.hh"
#include <gmock/gmock.h>

namespace test {

class MockEntityRegistry : public bsgalone::core::ForRegisteringEntity
{
  public:
  MockEntityRegistry()           = default;
  ~MockEntityRegistry() override = default;

  MOCK_METHOD(bsgalone::core::EntityId,
              registerAsteroid,
              (const bsgalone::core::Asteroid &),
              (override));
};

} // namespace test
