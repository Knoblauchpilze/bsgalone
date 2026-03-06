
#pragma once

#include "CoreObject.hh"
#include "IComponent.hh"
#include <string>

namespace bsgalone::core {

class AbstractComponent : public IComponent, public ::core::CoreObject
{
  public:
  AbstractComponent(const ComponentType &type);
  ~AbstractComponent() override = default;

  auto type() const -> ComponentType override;

  virtual void update(const chrono::TickData &data) = 0;

  private:
  ComponentType m_componentType{};
};

} // namespace bsgalone::core
