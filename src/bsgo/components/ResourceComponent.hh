
#pragma once

#include "AbstractComponent.hh"
#include "Uuid.hh"

namespace bsgo {

class ResourceComponent : public AbstractComponent
{
  public:
  ResourceComponent(const Uuid resource, const int amount);
  ~ResourceComponent() override = default;

  auto resource() const -> Uuid;
  auto amount() const -> int;

  void update(const chrono::TickData &data) override;

  void setAmount(const int amount);

  private:
  Uuid m_resource;
  int m_amount;
};

using ResourceComponentShPtr = std::shared_ptr<ResourceComponent>;

} // namespace bsgo
