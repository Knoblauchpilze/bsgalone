
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
    case Status::JUMP:
      return "jump";
    case Status::JUMP_APPEARING:
      return "jumpAppearing";
    case Status::JUMP_THREAT:
      return "jumpThreat";
    case Status::DEAD:
      return "dead";
    default:
      return "unknown";
  }
}

bool statusVisibleFromDradis(const Status &status)
{
  switch (status)
  {
    case Status::VISIBLE:
    case Status::THREAT:
    case Status::JUMP:
    case Status::JUMP_THREAT:
      return true;
    default:
      return false;
  }
}

bool statusAllowsInteratction(const Status &status)
{
  switch (status)
  {
    case Status::VISIBLE:
    case Status::THREAT:
    case Status::APPEARING:
    case Status::JUMP_APPEARING:
    case Status::JUMP:
    case Status::JUMP_THREAT:
      return true;
    default:
      return false;
  }
}

bool statusAllowsHealthRegeneration(const Status &status)
{
  switch (status)
  {
    case Status::THREAT:
    case Status::DEAD:
      return false;
    default:
      return true;
  }
}

bool statusAllowsPowerRegeneration(const Status &status)
{
  switch (status)
  {
    case Status::JUMP:
    case Status::JUMP_APPEARING:
    case Status::JUMP_THREAT:
      return false;
    default:
      return true;
  }
}

bool statusAllowsDamage(const Status &status)
{
  switch (status)
  {
    case Status::DOCKED:
    case Status::APPEARING:
    case Status::DEAD:
      return false;
    default:
      return true;
  }
}

bool statusAllowsDocking(const Status &status)
{
  switch (status)
  {
    case Status::APPEARING:
    case Status::VISIBLE:
    case Status::THREAT:
      return true;
    default:
      return false;
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

bool statusRequiresPowerReset(const Status &status)
{
  switch (status)
  {
    case Status::JUMP:
    case Status::JUMP_APPEARING:
    case Status::JUMP_THREAT:
      return true;
    default:
      return false;
  }
}

bool statusRequiresDeletion(const Status &status)
{
  switch (status)
  {
    case Status::DEAD:
    case Status::DOCKED:
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
    case Status::JUMP_THREAT:
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
    case Status::JUMP_APPEARING:
      return true;
    default:
      return false;
  }
}

bool statusIndicatesJump(const Status &status)
{
  switch (status)
  {
    case Status::JUMP:
    case Status::JUMP_APPEARING:
    case Status::JUMP_THREAT:
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
      return Status::THREAT;
    case Status::JUMP:
    case Status::JUMP_APPEARING:
      return Status::JUMP_THREAT;
    case Status::THREAT:
    case Status::JUMP_THREAT:
      return in;
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
    case Status::JUMP_APPEARING:
      return Status::JUMP;
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
    case Status::JUMP_THREAT:
      return Status::JUMP;
    default:
      throw std::invalid_argument("invalid status (" + str(in) + ") to clear threat");
  }
}

auto updateStatusForJump(const Status &in) -> Status
{
  switch (in)
  {
    case Status::APPEARING:
      return Status::JUMP_APPEARING;
    case Status::VISIBLE:
      return Status::JUMP;
    case Status::THREAT:
      return Status::JUMP_THREAT;
    default:
      throw std::invalid_argument("invalid status (" + str(in) + ") to start jump");
  }
}

auto updateStatusAfterJumpCancellation(const Status &in) -> Status
{
  switch (in)
  {
    case Status::JUMP_APPEARING:
      return Status::APPEARING;
    case Status::JUMP:
      return Status::VISIBLE;
    case Status::JUMP_THREAT:
      return Status::THREAT;
    default:
      throw std::invalid_argument("invalid status (" + str(in) + ") to cancel jump");
  }
}

} // namespace bsgo
