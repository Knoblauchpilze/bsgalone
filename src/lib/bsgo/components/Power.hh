
#pragma once

#include <memory>
#include <optional>

namespace bsgo {

class Power
{
  public:
  Power() = default;
  Power(const float current, const float max, const std::optional<float> min = {});
  ~Power() = default;

  auto power() const -> float;
  auto max() const -> float;

  private:
  float m_min{0.0f};
  float m_current{1.0f};
  float m_max{1.0f};

  void validate();
};

using PowerShPtr = std::shared_ptr<Power>;

} // namespace bsgo
