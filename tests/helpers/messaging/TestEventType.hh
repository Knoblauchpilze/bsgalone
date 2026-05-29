
#pragma once

#include <string>
#include <unordered_set>

namespace test {

enum class TestEventType
{
  EVENT_1,
  EVENT_2,
};

auto str(const TestEventType type) -> std::string;

auto allEventTypesAsSet() -> std::unordered_set<TestEventType>;

} // namespace test
