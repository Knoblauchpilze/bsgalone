
#pragma once

#include <string>
#include <unordered_set>

namespace bsgalone::client {

enum class UiEventType
{
  EXIT_REQUESTED,
  JUMP_REQUESTED,
  LOGIN_REQUESTED,
  SYSTEM_SELECTED,
  SYSTEMS_LIST_RECEIVED,
};

auto str(const UiEventType type) -> std::string;

auto allUiEventTypesAsSet() -> std::unordered_set<UiEventType>;

} // namespace bsgalone::client
