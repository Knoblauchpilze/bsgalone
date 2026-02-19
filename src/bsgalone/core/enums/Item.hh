
#pragma once

#include <string>

namespace bsgalone::core {

enum class Item
{
  RESOURCE,
  WEAPON,
  COMPUTER,
  SHIP
};

auto str(const Item item) -> std::string;

} // namespace bsgalone::core
