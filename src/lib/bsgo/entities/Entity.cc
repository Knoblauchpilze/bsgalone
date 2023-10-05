
#include "Entity.hh"

namespace bsgo {

auto Entity::str() const noexcept -> std::string
{
  std::string out("[");

  out += std::to_string(uuid);
  out += ",";
  out += bsgo::str(kind);

  if (transform)
  {
    const auto p = (*transform)->position();
    out += ",";
    out += std::to_string(p(0));
    out += "x";
    out += std::to_string(p(1));
    out += "x";
    out += std::to_string(p(2));
  }

  out += "]";
  return out;
}

} // namespace bsgo
