
#pragma once

#include "ForCreatingAsteroid.hh"
#include <gmock/gmock.h>

namespace test {

class MockAsteroidCreator : public bsgalone::core::ForCreatingAsteroid
{
  public:
  MockAsteroidCreator()           = default;
  ~MockAsteroidCreator() override = default;

  MOCK_METHOD(bsgalone::core::Uuid, create, (const bsgalone::core::Asteroid &), (override));
};

} // namespace test