
#pragma once

#include <string>
#include <unordered_set>

namespace bsgalone::client {

enum class UiEventType
{
  GAME_READY,
  HANGAR_READY,
  LOCKER_READY,
  LOGIN_FAILED,
  LOGIN_SUCCEEDED,
  LOGOUT,
  SHOP_READY,
  SIGNUP_FAILED,
  SIGNUP_SUCCEEDED,
  UNDOCK
};

auto str(const UiEventType type) -> std::string;

auto allUiEventTypesAsSet() -> std::unordered_set<UiEventType>;

} // namespace bsgalone::client
