
#include "TestEvent.hh"

namespace test {

TestEvent::TestEvent(const TestEventType type)
  : m_type(type)
{}

auto TestEvent::type() const -> TestEventType
{
  return m_type;
}

auto TestEvent::clone() const -> std::unique_ptr<TestEvent>
{
  return std::make_unique<TestEvent>(m_type);
}

} // namespace test
