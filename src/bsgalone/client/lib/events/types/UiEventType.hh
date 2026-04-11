
#pragma once

#include <string>
#include <unordered_set>

namespace bsgalone::client {

enum class UiEventType
{
  LOGIN_FAILED,
  LOGIN_SUCCEEDED,
  LOGOUT,
  SIGNUP_FAILED,
  SIGNUP_SUCCEEDED
};

auto str(const UiEventType type) -> std::string;

auto allUiEventTypesAsSet() -> std::unordered_set<UiEventType>;

} // namespace bsgalone::client
