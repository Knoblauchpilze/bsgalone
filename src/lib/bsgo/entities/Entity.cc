
#include "Entity.hh"

namespace bsgo {

auto Entity::str() const noexcept -> std::string
{
  std::string out("[");

  out += std::to_string(uuid);
  out += ",";
  out += bsgo::str(kind);

  out += "]";
  return out;
}

} // namespace bsgo
