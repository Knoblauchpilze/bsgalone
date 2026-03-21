
#include "TestEventPublisher.hh"

namespace test {

void TestEventPublisher::publishEvent(bsgalone::core::IGameEventPtr event)
{
  m_queue.pushEvent(std::move(event));
}

auto TestEventPublisher::queue() const -> const TestGameEventQueue &
{
  return m_queue;
}

} // namespace test
