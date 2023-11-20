
#pragma once

#include <string>

namespace bsgo {

enum class Status
{
  VISIBLE,
  APPEARING,
  DOCKED,
  THREAT,
  DEAD
};

auto str(const Status &status) -> std::string;

bool isInteractableStatus(const Status &status);
bool isVisibleStatus(const Status &status);

} // namespace bsgo
