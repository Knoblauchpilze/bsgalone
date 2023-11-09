

#pragma once

#include "IComponent.hh"
#include "Uuid.hh"

namespace bsgo {

enum class OwnerType
{
  PLAYER,
  SHIP,
};

class OwnerComponent : public IComponent
{
  public:
  OwnerComponent(const Uuid &owner, const OwnerType &ownerType);
  ~OwnerComponent() override = default;

  auto owner() const -> Uuid;
  auto type() const -> OwnerType;

  void update(const float elapsedSeconds) override;

  private:
  Uuid m_owner;
  OwnerType m_ownerType;
};

using OwnerComponentShPtr = std::shared_ptr<OwnerComponent>;

} // namespace bsgo
