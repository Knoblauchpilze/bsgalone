
#include "UiEventType.hh"

namespace bsgalone::client {

auto str(const UiEventType type) -> std::string
{
  switch (type)
  {
    case UiEventType::SYSTEMS_DATA_READY:
      return "systems_data_ready";
    default:
      return "unknown";
  }
}

} // namespace bsgalone::client
