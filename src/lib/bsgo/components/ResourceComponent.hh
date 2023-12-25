
#pragma once

#include "AbstractComponent.hh"
#include "Uuid.hh"

namespace bsgo {

class ResourceComponent : public AbstractComponent
{
  public:
  ResourceComponent(const Uuid &resource, const float amount);
  ~ResourceComponent() override = default;

  auto resource() const -> Uuid;
  auto amount() const -> float;

  void update(const float elapsedSeconds) override;

  void setAmount(const float amount);

  private:
  Uuid m_resource;
  float m_amount;
};

using ResourceComponentShPtr = std::shared_ptr<ResourceComponent>;

} // namespace bsgo
