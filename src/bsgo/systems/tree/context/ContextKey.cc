
#include "ContextKey.hh"

namespace bsgo {

auto str(const ContextKey &key) -> std::string
{
  switch (key)
  {
    case ContextKey::TARGET_REACHED:
      return "target_reached";
    default:
      return "unknown";
  }
}

} // namespace bsgo
