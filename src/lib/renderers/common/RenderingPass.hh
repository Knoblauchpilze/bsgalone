
#pragma once

#include <string>

namespace pge {

enum class RenderingPass
{
  SPRITES,
  DECAL,
  UI,
  DEBUG,
};

auto str(const RenderingPass pass) -> std::string;

} // namespace pge
