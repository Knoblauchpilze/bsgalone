
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
    default:
      return "unknown";
  };
}

} // namespace bsgo
