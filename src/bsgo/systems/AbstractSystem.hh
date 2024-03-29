
#pragma once

#include "Entity.hh"
#include "ISystem.hh"

namespace bsgo {

class Coordinator;

class AbstractSystem : public ISystem
{
  public:
  AbstractSystem(const SystemType &type, const EntityPredicate &entitiesFilter);
  AbstractSystem(const SystemType &type,
                 const EntityPredicate &entitiesFilter,
                 const bool filterDeadAndRemovedEntities);
  ~AbstractSystem() override = default;

  auto type() const -> SystemType override;
  void installInternalMessageQueue(IMessageQueue *messageQueue) override;
  void installOutputMessageQueue(IMessageQueue *messageQueue) override;

  void update(Coordinator &coordinator, const float elapsedSeconds) const override;

  virtual void updateEntity(Entity &entity,
                            Coordinator &coordinator,
                            const float elapsedSeconds) const = 0;

  protected:
  void pushInternalMessage(IMessagePtr message) const;
  void pushMessage(IMessagePtr message) const;

  private:
  SystemType m_systemType{};
  EntityPredicate m_entitiesFilter{};
  bool m_filterDeadAndRemovedEntities{true};
  IMessageQueue *m_internalMessageQueue{};
  IMessageQueue *m_outputMessageQueue{};
};

} // namespace bsgo
