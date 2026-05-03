
#include "EntityId.hh"

namespace bsgalone::core {

auto str(const EntityId id) -> std::string
{
  return std::to_string(id);
}

} // namespace bsgalone::core