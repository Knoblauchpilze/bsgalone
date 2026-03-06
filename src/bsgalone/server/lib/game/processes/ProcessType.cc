
#include "ProcessType.hh"

namespace bsgalone::server {

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

} // namespace bsgalone::server
