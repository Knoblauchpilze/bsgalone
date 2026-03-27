
#include "UiCommandType.hh"

namespace bsgalone::client {

auto str(const UiCommandType type) -> std::string
{
  switch (type)
  {
    case UiCommandType::EXIT_REQUESTED:
      return "exit_requested";
    case UiCommandType::LOGIN_REQUESTED:
      return "login_requested";
    default:
      return "unknown";
  }
}

auto allUiCommandTypesAsSet() -> std::unordered_set<UiCommandType>
{
  return std::unordered_set<UiCommandType>{
    UiCommandType::EXIT_REQUESTED,
    UiCommandType::LOGIN_REQUESTED,
  };
}

} // namespace bsgalone::client
