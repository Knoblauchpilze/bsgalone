
#include "TestEventType.hh"

namespace test {

auto str(const TestEventType type) -> std::string
{
  return std::to_string(static_cast<int>(type));
}

auto allEventTypesAsSet() -> std::unordered_set<TestEventType>
{
  return std::unordered_set<TestEventType>{
    TestEventType::EVENT_1,
    TestEventType::EVENT_2,
  };
}

} // namespace test
