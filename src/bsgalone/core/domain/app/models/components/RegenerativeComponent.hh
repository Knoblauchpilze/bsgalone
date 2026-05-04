
#pragma once

namespace bsgalone::core {

struct RegenerativeComponent
{
  float min{0.0f};
  float value{0.0f};
  float max{0.0f};

  float regenPerTick{0.0f};
};

} // namespace bsgalone::core
