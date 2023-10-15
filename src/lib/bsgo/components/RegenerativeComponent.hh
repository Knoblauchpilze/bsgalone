
#pragma once

#include "IComponent.hh"
#include <memory>

namespace bsgo {

class RegenerativeComponent : public IComponent
{
  RegenerativeComponent(const std::string &name,
                        const float min,
                        const float value,
                        const float max,
                        const float regenPerSecond);
  ~RegenerativeComponent() override = default;

  void update(const float elapsedSeconds) override;

  private:
  float m_min;
  float m_value;
  float m_max;

  float m_regenPerSecond;

  void validate();
};

} // namespace bsgo
