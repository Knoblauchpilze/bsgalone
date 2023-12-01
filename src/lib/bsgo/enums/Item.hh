
#pragma once

#include <string>

namespace bsgo {

enum class Item
{
  RESOURCE,
  WEAPON,
  COMPUTER
};

auto str(const Item item) -> std::string;

} // namespace bsgo
