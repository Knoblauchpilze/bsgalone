
#include "TestEvent.hh"

namespace test {

TestEvent::TestEvent(const TestEventType type)
  : TestEvent(type, 0)
{}

TestEvent::TestEvent(const TestEventType type, const int index)
  : m_type(type)
  , m_index(index)
{}

auto TestEvent::type() const -> TestEventType
{
  return m_type;
}

auto TestEvent::clone() const -> std::unique_ptr<TestEvent>
{
  return std::make_unique<TestEvent>(m_type, m_index);
}

} // namespace test
