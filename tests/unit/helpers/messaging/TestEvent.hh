
#pragma once

#include "TestEventType.hh"
#include <memory>

namespace test {

class TestEvent
{
  public:
  TestEvent(const TestEventType type);

  auto type() const -> TestEventType;

  auto clone() const -> std::unique_ptr<TestEvent>;

  private:
  TestEventType m_type{};
};

using TestEventPtr = std::unique_ptr<TestEvent>;

} // namespace test
