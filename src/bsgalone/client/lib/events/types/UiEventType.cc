
#include "UiEventType.hh"

namespace bsgalone::client {

auto str(const UiEventType type) -> std::string
{
  switch (type)
  {
    case UiEventType::LOGIN_FAILED:
      return "exit_failed";
    case UiEventType::LOGIN_SUCCEEDED:
      return "login_succeeded";
    default:
      return "unknown";
  }
}

auto allUiEventTypesAsSet() -> std::unordered_set<UiEventType>
{
  return std::unordered_set<UiEventType>{
    UiEventType::LOGIN_FAILED,
    UiEventType::LOGIN_SUCCEEDED,
  };
}

} // namespace bsgalone::client
