
#pragma once

#include "IComponent.hh"

namespace bsgo {

class RegenerativeComponent : public IComponent
{
  public:
  RegenerativeComponent(const std::string &name,
                        const float min,
                        const float value,
                        const float max,
                        const float regenPerSecond);
  ~RegenerativeComponent() override = default;

  void update(const float elapsedSeconds) override;

  auto min() const -> float;
  auto value() const -> float;
  auto max() const -> float;

  void use(const float amount);

  private:
  float m_min;
  float m_value;
  float m_max;

  float m_regenPerSecond;

  void validate();
};

} // namespace bsgo
