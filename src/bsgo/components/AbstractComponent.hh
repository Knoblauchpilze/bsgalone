
#pragma once

#include "CoreObject.hh"
#include "IComponent.hh"
#include <string>

namespace bsgo {

class AbstractComponent : public bsgalone::core::IComponent, public ::core::CoreObject
{
  public:
  AbstractComponent(const bsgalone::core::ComponentType &type);
  ~AbstractComponent() override = default;

  auto type() const -> bsgalone::core::ComponentType override;

  private:
  bsgalone::core::ComponentType m_componentType{};
};

} // namespace bsgo
