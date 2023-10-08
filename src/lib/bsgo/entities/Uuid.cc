
#include "Uuid.hh"

namespace bsgo {

auto str(const Uuid &id) -> std::string
{
  return std::to_string(id);
}

} // namespace bsgo
