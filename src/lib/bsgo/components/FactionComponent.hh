
#pragma once

#include "Faction.hh"
#include "IComponent.hh"

namespace bsgo {

class FactionComponent : public IComponent
{
  public:
  FactionComponent(const Faction &faction);
  ~FactionComponent() override = default;

  auto faction() const noexcept -> Faction;

  void update(const float elapsedSeconds) override;

  private:
  Faction m_faction;
};

using FactionComponentShPtr = std::shared_ptr<FactionComponent>;

} // namespace bsgo
