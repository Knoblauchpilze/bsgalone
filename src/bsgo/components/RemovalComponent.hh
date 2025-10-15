
#pragma once

#include "AbstractComponent.hh"

namespace bsgo {

class RemovalComponent : public AbstractComponent
{
  public:
  RemovalComponent();
  ~RemovalComponent() override = default;

  void markForRemoval(const bool toRemove = true);
  bool toBeDeleted() const;

  void update(const chrono::TickData &data) override;

  private:
  bool m_markedForRemoval{false};
};

using RemovalComponentShPtr = std::shared_ptr<RemovalComponent>;

} // namespace bsgo
