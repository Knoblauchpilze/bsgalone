
#pragma once

#include "AbstractComponent.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgo {

class TargetComponent : public AbstractComponent
{
  public:
  TargetComponent();
  TargetComponent(const Uuid target);
  ~TargetComponent() override = default;

  auto target() const -> std::optional<Uuid>;
  void clearTarget();
  void setTarget(const Uuid target);

  void update(const chrono::TickData &data) override;

  private:
  std::optional<Uuid> m_target{};
};

using TargetComponentShPtr = std::shared_ptr<TargetComponent>;

} // namespace bsgo
