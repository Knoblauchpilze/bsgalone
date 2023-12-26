
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
  void installMessageQueue(IMessageQueue *messageQueue) override;

  void update(Coordinator &coordinator, const float elapsedSeconds) const override;

  virtual void updateEntity(Entity &entity,
                            Coordinator &coordinator,
                            const float elapsedSeconds) const = 0;

  protected:
  void pushMessage(IMessagePtr message);

  private:
  SystemType m_systemType{};
  Coordinator::EntityPredicate m_entitiesFilter{};
  IMessageQueue *m_messageQueue{};
};

} // namespace bsgo
