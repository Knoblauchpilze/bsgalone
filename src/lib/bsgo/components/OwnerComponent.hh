

#pragma once

#include "IComponent.hh"
#include "Uuid.hh"

namespace bsgo {

class OwnerComponent : public IComponent
{
  public:
  OwnerComponent(const Uuid &owner);
  ~OwnerComponent() override = default;

  auto owner() const -> Uuid;

  void update(const float elapsedSeconds) override;

  private:
  Uuid m_owner;
};

using OwnerComponentShPtr = std::shared_ptr<OwnerComponent>;

} // namespace bsgo
