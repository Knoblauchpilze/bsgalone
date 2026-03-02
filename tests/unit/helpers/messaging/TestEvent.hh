
#pragma once

#include "TestEventType.hh"
#include <memory>

namespace test {

class TestEvent
{
  public:
  TestEvent(const TestEventType type);
  TestEvent(const TestEventType type, const int index);

  auto type() const -> TestEventType;

  auto clone() const -> std::unique_ptr<TestEvent>;

  private:
  TestEventType m_type{};
  int m_index{};
};

using TestEventPtr = std::unique_ptr<TestEvent>;

} // namespace test
