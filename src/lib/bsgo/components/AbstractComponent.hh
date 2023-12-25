
#pragma once

#include "ComponentType.hh"
#include "IComponent.hh"
#include <string>

namespace bsgo {

class AbstractComponent : public IComponent
{
  public:
  AbstractComponent(const std::string &name, const ComponentType &type);
  ~AbstractComponent() override = default;

  auto type() const -> ComponentType override;

  private:
  ComponentType m_componentType{};
};

} // namespace bsgo
