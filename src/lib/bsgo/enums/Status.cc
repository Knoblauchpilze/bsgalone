
#include "Status.hh"

namespace bsgo {

auto str(const Status &status) -> std::string
{
  switch (status)
  {
    case Status::VISIBLE:
      return "visible";
    case Status::APPEARING:
      return "appearing";
    case Status::DOCKED:
      return "docked";
    case Status::DEAD:
      return "dead";
    default:
      return "unknown";
  };
}

} // namespace bsgo
