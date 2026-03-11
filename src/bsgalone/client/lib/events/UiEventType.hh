
#pragma once

#include <string>

namespace bsgalone::client {

enum class UiEventType
{
  SYSTEMS_DATA_READY
};

auto str(const UiEventType type) -> std::string;

} // namespace bsgalone::client
