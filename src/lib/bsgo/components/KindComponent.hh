
#pragma once

#include "EntityKind.hh"
#include "IComponent.hh"

namespace bsgo {

class KindComponent : public IComponent
{
  public:
  KindComponent(const EntityKind &kind);
  ~KindComponent() = default;

  auto kind() const noexcept -> EntityKind;

  void update(const float elapsedSeconds) override;

  private:
  EntityKind m_kind;
};

using KindComponentShPtr = std::shared_ptr<KindComponent>;

} // namespace bsgo
