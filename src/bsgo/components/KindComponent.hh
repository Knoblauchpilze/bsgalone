
#pragma once

#include "AbstractComponent.hh"
#include "EntityKind.hh"

namespace bsgo {

class KindComponent : public AbstractComponent
{
  public:
  KindComponent(const bsgalone::core::EntityKind &kind);
  ~KindComponent() override = default;

  auto kind() const noexcept -> bsgalone::core::EntityKind;

  void update(const chrono::TickData &data) override;

  private:
  bsgalone::core::EntityKind m_kind;
};

using KindComponentShPtr = std::shared_ptr<KindComponent>;

} // namespace bsgo
