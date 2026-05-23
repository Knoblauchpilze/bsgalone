
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
};

} // namespace test
