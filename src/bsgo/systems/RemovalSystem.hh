
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class RemovalSystem : public AbstractSystem
{
  public:
  RemovalSystem();
  ~RemovalSystem() override = default;

  protected:
  void updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const override;

  private:
  void markEntityForRemoval(Entity &entity) const;
};

} // namespace bsgo
