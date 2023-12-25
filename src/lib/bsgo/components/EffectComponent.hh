
#pragma once

#include "AbstractComponent.hh"
#include <core_utils/TimeUtils.hh>
#include <optional>

namespace bsgo {

class EffectComponent : public AbstractComponent
{
  public:
  EffectComponent(const ComponentType &type, const utils::Duration &duration);
  ~EffectComponent() override = default;

  bool isFinished() const;

  virtual auto damageModifier() const -> std::optional<float>;

  void update(const float elapsedSeconds) override;

  private:
  utils::Duration m_duration;
  utils::Duration m_elapsedSinceStart{};
};

using EffectComponentShPtr = std::shared_ptr<EffectComponent>;

} // namespace bsgo
