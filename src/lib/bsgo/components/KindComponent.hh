
#pragma once

#include "AbstractComponent.hh"
#include "EntityKind.hh"

namespace bsgo {

class KindComponent : public AbstractComponent
{
  public:
  KindComponent(const EntityKind &kind);
  ~KindComponent() override = default;

  auto kind() const noexcept -> EntityKind;

  void update(const float elapsedSeconds) override;

  private:
  EntityKind m_kind;
};

using KindComponentShPtr = std::shared_ptr<KindComponent>;

} // namespace bsgo
