
#pragma once

#include "IComponent.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgo {

class TargetComponent : public IComponent
{
  public:
  TargetComponent();
  TargetComponent(const Uuid &target);
  ~TargetComponent() = default;

  auto target() const -> std::optional<Uuid>;
  void clearTarget();
  void setTarget(const Uuid &target);

  void update(const float elapsedSeconds) override;

  private:
  std::optional<Uuid> m_target{};
};

using TargetComponentShPtr = std::shared_ptr<TargetComponent>;

} // namespace bsgo
