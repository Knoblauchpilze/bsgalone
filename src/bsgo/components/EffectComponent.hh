
#pragma once

#include "AbstractComponent.hh"
#include "TimeUtils.hh"
#include <optional>

namespace bsgo {

class EffectComponent : public AbstractComponent
{
  public:
  EffectComponent(const ComponentType &type, const core::Duration &duration);
  ~EffectComponent() override = default;

  bool isFinished() const;

  virtual auto damageModifier() const -> std::optional<float>;

  void update(const float elapsedSeconds) override;

  private:
  core::Duration m_duration;
  core::Duration m_elapsedSinceStart{};
};

using EffectComponentShPtr = std::shared_ptr<EffectComponent>;

} // namespace bsgo
