

#pragma once

#include "IComponent.hh"

namespace bsgo {

class LockerComponent : public IComponent
{
  public:
  LockerComponent();
  ~LockerComponent() override = default;

  void update(const float elapsedSeconds) override;
};

using LockerComponentShPtr = std::shared_ptr<LockerComponent>;

} // namespace bsgo
