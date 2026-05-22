
#include "SystemProcessor.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::server {
namespace {
class MockEcsCoordinator : public core::ForRunningSimulation
{
  public:
  MockEcsCoordinator()           = default;
  ~MockEcsCoordinator() override = default;

  MOCK_METHOD(void, update, (const chrono::TickData &), (override));
};

class MockTimeManager : public chrono::ITimeManager
{
  public:
  MockTimeManager()           = default;
  ~MockTimeManager() override = default;

  MOCK_METHOD(chrono::TickData, tick, (const chrono::Duration &), (override));
};

void runSystemProcessor(SystemProcessor &processor)
{
  processor.start();

  // This duration should be long enough to get at least one call to the
  // simulation update.
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  processor.stop();
}
} // namespace

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driven_SystemProcessor, ThrowsWhenEcsCoordinatorIsNull)
{
  EXPECT_THROW(
    []() { SystemProcessor("test-system", nullptr, std::make_unique<MockTimeManager>()); }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driven_SystemProcessor, ThrowsWhenTimeManagerIsNull)
{
  EXPECT_THROW(
    []() { SystemProcessor("test-system", std::make_unique<MockEcsCoordinator>(), nullptr); }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driven_SystemProcessor, ProvidesElapsedTimeInMilliseconds)
{
  auto manager = std::make_unique<StrictMock<MockTimeManager>>();
  std::vector<chrono::Duration> captured{};
  EXPECT_CALL(*manager, tick(_))
    .Times(AtLeast(1))
    .WillRepeatedly(Invoke([&captured](const chrono::Duration &elapsed) {
      captured.push_back(elapsed);
      return chrono::TickData{};
    }));

  SystemProcessor processor("test-system",
                            std::make_unique<NiceMock<MockEcsCoordinator>>(),
                            std::move(manager));

  runSystemProcessor(processor);

  for (const auto &elapsed : captured)
  {
    EXPECT_EQ(chrono::Unit::MILLISECONDS, elapsed.unit);
    // Each time step should be 50ms. The test allows for 25% variation.
    EXPECT_LT(25, elapsed.elapsed);
    EXPECT_LT(elapsed.elapsed, 75);
  }
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driven_SystemProcessor,
     TriggersSimulationUpdatePeriodically)
{
  auto manager = std::make_unique<StrictMock<MockTimeManager>>();
  chrono::TickData testTickData{
    .tick    = chrono::Tick::fromInt(2),
    .elapsed = chrono::TickDuration::fromInt(1),
  };
  EXPECT_CALL(*manager, tick(_)).Times(AtLeast(1)).WillRepeatedly(Return(testTickData));

  auto coordinator = std::make_unique<StrictMock<MockEcsCoordinator>>();
  EXPECT_CALL(*coordinator, update(testTickData)).Times(AtLeast(1));

  SystemProcessor processor("test-system", std::move(coordinator), std::move(manager));

  runSystemProcessor(processor);
}

} // namespace bsgalone::server
