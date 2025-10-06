
#pragma once

#include "AbstractComponent.hh"
#include "TickDuration.hh"
#include <optional>

namespace bsgo {

class EffectComponent : public AbstractComponent
{
  public:
  EffectComponent(const ComponentType &type, const TickDuration &duration);
  ~EffectComponent() override = default;

  bool isFinished() const;

  virtual auto damageModifier() const -> std::optional<float>;

  void update(const TickData &data) override;

  private:
  TickDuration m_duration;
  TickDuration m_elapsedSinceStart{};
};

using EffectComponentShPtr = std::shared_ptr<EffectComponent>;

} // namespace bsgo
