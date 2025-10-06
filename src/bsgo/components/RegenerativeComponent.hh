
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
                        const float regenPerTick);
  ~RegenerativeComponent() override = default;

  void update(const TickData &data) override;

  auto min() const -> float;
  auto value() const -> float;
  auto max() const -> float;

  void overrideValue(const float value);

  protected:
  void updateValue(const float delta);

  private:
  float m_min;
  float m_value;
  float m_max;

  float m_regenPerTick;

  void validate();
};

} // namespace bsgo
