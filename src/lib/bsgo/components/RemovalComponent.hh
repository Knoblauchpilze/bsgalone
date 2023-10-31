
#pragma once

#include "IComponent.hh"

namespace bsgo {

class RemovalComponent : public IComponent
{
  public:
  RemovalComponent();
  ~RemovalComponent() override = default;

  void markForRemoval(const bool toRemove = true);
  bool toBeDeleted() const;

  void update(const float elapsedSeconds) override;

  private:
  bool m_markedForRemoval{false};
};

using RemovalComponentShPtr = std::shared_ptr<RemovalComponent>;

} // namespace bsgo
