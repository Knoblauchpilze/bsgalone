
#pragma once

#include "IGame.hh"
#include <gmock/gmock.h>

namespace test {

class MockGame : public bsgalone::client::IGame
{
  public:
  MockGame()           = default;
  ~MockGame() override = default;

  MOCK_METHOD(void, onSystemDataReceived, (const bsgalone::client::SystemData &), (override));
  MOCK_METHOD(void, update, (const float), (override));
  MOCK_METHOD(void, reset, (), (override));
  MOCK_METHOD(std::vector<bsgalone::core::Asteroid>,
              getAsteroidsWithin,
              (const bsgalone::core::IBoundingBox &),
              (const, override));
};

} // namespace test
