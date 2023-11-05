
#pragma once

#include "IComponent.hh"

namespace bsgo {

class AIComponent : public IComponent
{
  public:
  AIComponent();
  ~AIComponent() = default;

  void update(const float elapsedSeconds) override;
};

using AIComponentShPtr = std::shared_ptr<AIComponent>;

} // namespace bsgo
