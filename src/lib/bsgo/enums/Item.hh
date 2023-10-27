
#pragma once

#include <string>

namespace bsgo {

enum class Item
{
  RESOURCE,
};

auto str(const Item item) -> std::string;

} // namespace bsgo
