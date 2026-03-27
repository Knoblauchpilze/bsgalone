
#pragma once

#include <string>
#include <unordered_set>

namespace bsgalone::client {

enum class UiCommandType
{
  EXIT_REQUESTED,
  LOGIN_REQUESTED,
  SIGNUP_REQUESTED
};

auto str(const UiCommandType type) -> std::string;

auto allUiCommandTypesAsSet() -> std::unordered_set<UiCommandType>;

} // namespace bsgalone::client
