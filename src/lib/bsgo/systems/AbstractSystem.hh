
#pragma once

#include "Coordinator.hh"
#include "Entity.hh"
#include "ISystem.hh"

namespace bsgo {

class AbstractSystem : public ISystem
{
  public:
  AbstractSystem(const SystemType &type, const Coordinator::EntityPredicate &entitiesFilter);
  ~AbstractSystem() override = default;

  auto type() const -> SystemType override;

  void update(Coordinator &coordinator, const float elapsedSeconds) const override;

  virtual void updateEntity(Entity &entity,
                            Coordinator &coordinator,
                            const float elapsedSeconds) const = 0;

  private:
  SystemType m_systemType{};
  Coordinator::EntityPredicate m_entitiesFilter;
};

} // namespace bsgo
