
#pragma once

#include "IComponent.hh"
#include <string>

namespace bsgo {

class AbstractComponent : public IComponent
{
  public:
  AbstractComponent(const ComponentType &type);
  ~AbstractComponent() override = default;

  auto type() const -> ComponentType override;

  private:
  ComponentType m_componentType{};
};

} // namespace bsgo
