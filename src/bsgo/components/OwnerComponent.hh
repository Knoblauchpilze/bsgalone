

#pragma once

#include "AbstractComponent.hh"
#include "Uuid.hh"

namespace bsgo {

enum class OwnerType
{
  PLAYER,
  SHIP,
};

class OwnerComponent : public AbstractComponent
{
  public:
  OwnerComponent(const Uuid owner, const OwnerType &ownerType);
  ~OwnerComponent() override = default;

  auto owner() const -> Uuid;
  auto category() const -> OwnerType;

  void update(const TickData &data) override;

  private:
  Uuid m_owner;
  OwnerType m_ownerType;
};

using OwnerComponentShPtr = std::shared_ptr<OwnerComponent>;

} // namespace bsgo
