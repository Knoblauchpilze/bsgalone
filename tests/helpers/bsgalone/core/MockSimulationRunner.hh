
#pragma once

#include "ForRunningSimulation.hh"
#include <gmock/gmock.h>

namespace test {

class MockSimulationRunner : public bsgalone::core::ForRunningSimulation
{
  public:
  MockSimulationRunner()           = default;
  ~MockSimulationRunner() override = default;

  MOCK_METHOD(void, update, (const chrono::TickData &), (override));
};

} // namespace test