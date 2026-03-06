
#include "NodeState.hh"

namespace bsgalone::core {

auto str(const NodeState &state) -> std::string
{
  switch (state)
  {
    case NodeState::IDLE:
      return "idle";
    case NodeState::RUNNING:
      return "running";
    case NodeState::FAILED:
      return "failed";
    case NodeState::SUCCESS:
      return "success";
    default:
      return "unknown";
  }
}

} // namespace bsgalone::core
