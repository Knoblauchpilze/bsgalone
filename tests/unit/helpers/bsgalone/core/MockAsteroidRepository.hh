
#pragma once

#include "ForManagingAsteroid.hh"
#include <gmock/gmock.h>

namespace test {

class MockAsteroidRepository : public bsgalone::core::ForManagingAsteroid
{
  public:
  MockAsteroidRepository()           = default;
  ~MockAsteroidRepository() override = default;

  MOCK_METHOD(std::vector<bsgalone::core::Asteroid>,
              findAllBySystem,
              (const bsgalone::core::Uuid),
              (const, override));
};

} // namespace test
