
#pragma once

#include <string>

namespace bsgalone::core {

enum class NodeState
{
  IDLE,
  RUNNING,
  FAILED,
  SUCCESS
};

auto str(const NodeState &state) -> std::string;

} // namespace bsgalone::core
