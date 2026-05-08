
#pragma once

#include "ForManagingEntity.hh"
#include <gmock/gmock.h>

namespace test {

class MockEntityManager : public bsgalone::server::ForManagingEntity
{
  public:
  MockEntityManager()           = default;
  ~MockEntityManager() override = default;

  MOCK_METHOD(void, createAsteroid, (const bsgalone::core::Asteroid &), (override));
};

} // namespace test
