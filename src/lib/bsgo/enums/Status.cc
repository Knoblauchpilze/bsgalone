
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
    case Status::THREAT:
      return "threat";
    case Status::DEAD:
      return "dead";
    default:
      return "unknown";
  };
}

bool isInteractableStatus(const Status &status)
{
  switch (status)
  {
    case Status::VISIBLE:
    case Status::THREAT:
    case Status::APPEARING:
      return true;
    default:
      return false;
  }
}

bool isVisibleStatus(const Status &status)
{
  switch (status)
  {
    case Status::VISIBLE:
    case Status::THREAT:
      return true;
    default:
      return false;
  }
}

} // namespace bsgo
