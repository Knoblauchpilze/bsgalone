
#pragma once

#include "Coordinator.hh"
#include "Entity.hh"
#include "ISystem.hh"

namespace bsgo {

class AbstractSystem : public ISystem
{
  public:
  AbstractSystem(const std::string &name, const Coordinator::EntityPredicate &entitiesFilter);
  ~AbstractSystem() override = default;

  void update(const Coordinator &coordinator, const float elapsedSeconds) override;

  virtual void updateEntity(Entity &entity,
                            const Coordinator &coordinator,
                            const float elapsedSeconds)
    = 0;

  private:
  Coordinator::EntityPredicate m_entitiesFilter;
};

} // namespace bsgo
