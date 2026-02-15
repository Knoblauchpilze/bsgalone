
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
  void installInternalMessageQueue(bsgalone::core::IMessageQueue *messageQueue) override;
  void installOutputMessageQueue(bsgalone::core::IMessageQueue *messageQueue) override;

  void update(Coordinator &coordinator, const chrono::TickData &data) const override;

  protected:
  void pushInternalMessage(bsgalone::core::IMessagePtr message) const;
  void pushMessage(bsgalone::core::IMessagePtr message) const;

  virtual void updateEntity(Entity &entity,
                            Coordinator &coordinator,
                            const chrono::TickData &data) const = 0;

  private:
  SystemType m_systemType{};
  EntityPredicate m_entitiesFilter{};
  bool m_filterDeadAndRemovedEntities{true};
  bsgalone::core::IMessageQueue *m_internalMessageQueue{};
  bsgalone::core::IMessageQueue *m_outputMessageQueue{};
};

} // namespace bsgo
