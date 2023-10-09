
#pragma once

#include <memory>

namespace bsgo {

class HullPoints
{
  public:
  HullPoints() = default;
  HullPoints(const float current, const float max);
  ~HullPoints() = default;

  auto power() const -> float;
  auto max() const -> float;

  private:
  float m_current{1.0f};
  float m_max{1.0f};

  /// @brief - Verifies that the ranges are valid, throws an exception if not.
  void validate();
};

using HullPointsShPtr = std::shared_ptr<HullPoints>;

} // namespace bsgo
