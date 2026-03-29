
#pragma once

#include "ForPublishingEvent.hh"
#include "TestGameEventQueue.hh"

namespace test {

class TestGameEventPublisher : public bsgalone::core::ForPublishingEvent
{
  public:
  TestGameEventPublisher()           = default;
  ~TestGameEventPublisher() override = default;

  void publishEvent(bsgalone::core::IGameEventPtr event) override;

  /// @brief - Retrieves the underlying test game event queue which stores the
  /// messages received through the publishEvent public interface.
  /// @return - a game event queue.
  auto queue() const -> const TestGameEventQueue &;

  private:
  TestGameEventQueue m_queue{};
};

} // namespace test
