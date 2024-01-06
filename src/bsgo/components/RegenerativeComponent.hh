
#pragma once

#include "AbstractComponent.hh"

namespace bsgo {

class RegenerativeComponent : public AbstractComponent
{
  public:
  RegenerativeComponent(const ComponentType &type,
                        const float min,
                        const float value,
                        const float max,
                        const float regenPerSecond);
  ~RegenerativeComponent() override = default;

  void update(const float elapsedSeconds) override;

  auto min() const -> float;
  auto value() const -> float;
  auto max() const -> float;

  protected:
  void updateValue(const float delta);

  private:
  float m_min;
  float m_value;
  float m_max;

  float m_regenPerSecond;

  void validate();
};

} // namespace bsgo
