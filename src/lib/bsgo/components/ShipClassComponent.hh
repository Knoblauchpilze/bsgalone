
#pragma once

#include "IComponent.hh"
#include "ShipClass.hh"

namespace bsgo {

class ShipClassComponent : public IComponent
{
  public:
  ShipClassComponent(const ShipClass &shipClass);
  ~ShipClassComponent() override = default;

  auto shipClass() const noexcept -> ShipClass;

  void update(const float elapsedSeconds) override;

  private:
  ShipClass m_shipClass;
};

using ShipClassComponentShPtr = std::shared_ptr<ShipClassComponent>;

} // namespace bsgo
