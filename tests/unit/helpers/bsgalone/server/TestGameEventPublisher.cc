
#include "TestGameEventPublisher.hh"

namespace test {

void TestGameEventPublisher::publishEvent(bsgalone::core::IGameEventPtr event)
{
  m_queue.pushEvent(std::move(event));
}

auto TestGameEventPublisher::queue() const -> const TestGameEventQueue &
{
  return m_queue;
}

} // namespace test
