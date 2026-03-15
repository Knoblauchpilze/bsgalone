
#include "UiEventType.hh"

namespace bsgalone::client {

auto str(const UiEventType type) -> std::string
{
  switch (type)
  {
    case UiEventType::JUMP_REQUESTED:
      return "jump_requested";
    case UiEventType::SYSTEM_SELECTED:
      return "system_selected";
    case UiEventType::SYSTEMS_LIST_RECEIVED:
      return "systems_list_received";
    default:
      return "unknown";
  }
}

auto allUiEventTypesAsSet() -> std::unordered_set<UiEventType>
{
  return std::unordered_set<UiEventType>{
    UiEventType::JUMP_REQUESTED,
    UiEventType::SYSTEM_SELECTED,
    UiEventType::SYSTEMS_LIST_RECEIVED,
  };
}

} // namespace bsgalone::client
