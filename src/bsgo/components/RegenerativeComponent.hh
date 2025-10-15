
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

  void update(const chrono::TickData &data) override;

  auto min() const -> float;
  auto value() const -> float;
  auto max() const -> float;

  void overrideValue(const float value);

  protected:
  void updateValue(const float delta);

  private:
  float m_min{0.0f};
  float m_value{0.0f};
  float m_max{0.0f};

  float m_regenPerTick{0.0f};

  void validate();
};

} // namespace bsgo
