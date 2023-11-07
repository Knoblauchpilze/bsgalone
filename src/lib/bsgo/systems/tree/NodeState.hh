
#pragma once

#include <string>

namespace bsgo {

enum class NodeState
{
  IDLE,
  RUNNING,
  FAILED,
  SUCCESS
};

auto str(const NodeState &state) -> std::string;

} // namespace bsgo
