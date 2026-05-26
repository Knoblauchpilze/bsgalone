
#pragma once

#include "ForFetchingAsteroid.hh"
#include <gmock/gmock.h>

namespace test {

class MockAsteroidFetcher : public bsgalone::core::ForFetchingAsteroid
{
  public:
  MockAsteroidFetcher()           = default;
  ~MockAsteroidFetcher() override = default;

  MOCK_METHOD(std::vector<bsgalone::core::Asteroid>, getAllAsteroids, (), (const, override));
};

} // namespace test