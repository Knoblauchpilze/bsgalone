
#include "ProcessType.hh"

namespace bsgo {

auto str(const ProcessType &type) -> std::string
{
  switch (type)
  {
    case ProcessType::DB_SYNC:
      return "db_sync";
    case ProcessType::RESPAWN:
      return "respawn";
    case ProcessType::TICK_SYNC:
      return "tick_sync";
    default:
      return "unknown";
  }
}

} // namespace bsgo
