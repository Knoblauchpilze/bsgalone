
#pragma once

#include "AbstractComponent.hh"
#include "ShipClass.hh"

namespace bsgo {

class ShipClassComponent : public AbstractComponent
{
  public:
  ShipClassComponent(const ShipClass &shipClass);
  ~ShipClassComponent() override = default;

  auto shipClass() const noexcept -> ShipClass;

  void update(const TickData &data) override;

  private:
  ShipClass m_shipClass;
};

using ShipClassComponentShPtr = std::shared_ptr<ShipClassComponent>;

} // namespace bsgo
