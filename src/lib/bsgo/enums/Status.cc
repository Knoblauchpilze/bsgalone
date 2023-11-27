
#include "Status.hh"
#include <stdexcept>

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
  }
}

bool statusAllowsInteratction(const Status &status)
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

bool statusVisibleFromDradis(const Status &status)
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

bool statusAllowsRegeneration(const Status &status)
{
  switch (status)
  {
    case Status::THREAT:
      return false;
    default:
      return true;
  }
}

bool statusRequiresImmobilization(const Status &status)
{
  switch (status)
  {
    case Status::DOCKED:
    case Status::DEAD:
      return true;
    default:
      return false;
  }
}

bool statusIndicatesThreat(const Status &status)
{
  switch (status)
  {
    case Status::THREAT:
      return true;
    default:
      return false;
  }
}

bool statusIndicatesAppearing(const Status &status)
{
  switch (status)
  {
    case Status::APPEARING:
      return true;
    default:
      return false;
  }
}

bool statusRequiresThreatReset(const Status &status)
{
  switch (status)
  {
    case Status::DOCKED:
      return true;
    default:
      return false;
  }
}

auto updateStatusWithThreat(const Status &in) -> Status
{
  switch (in)
  {
    case Status::VISIBLE:
    case Status::APPEARING:
    case Status::THREAT:
      return Status::THREAT;
    default:
      throw std::invalid_argument("invalid status (" + str(in) + ") to go in threat");
  }
}

auto updateStatusAfterSpawn(const Status &in) -> Status
{
  switch (in)
  {
    case Status::APPEARING:
      return Status::VISIBLE;
    default:
      throw std::invalid_argument("invalid status (" + str(in) + ") to spawn");
  }
}

auto updateStatusAfterThreatEnded(const Status &in) -> Status
{
  switch (in)
  {
    case Status::THREAT:
      return Status::VISIBLE;
    case Status::DOCKED:
      return in;
    default:
      throw std::invalid_argument("invalid status (" + str(in) + ")to clear threat");
  }
}

} // namespace bsgo
