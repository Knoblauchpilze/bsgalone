
#pragma once

#include "IComponent.hh"
#include <core_utils/TimeUtils.hh>
#include <optional>

namespace bsgo {

struct SlotComponentData
{
  bool offensive;
  float powerCost;
  float range;
  utils::Duration reloadTime;
};

class SlotComponent : public IComponent
{
  public:
  SlotComponent(const std::string &name, const SlotComponentData &data);
  ~SlotComponent() override = default;

  void update(const float elapsedSeconds) override;

  bool isOffensive() const;
  auto powerCost() const -> float;
  auto range() const -> float;
  bool canFire() const noexcept;
  auto reloadPercentage() const -> float;

  void fire();

  private:
  bool m_offensive;
  float m_powerCost;
  float m_range;
  utils::Duration m_reloadTime;

  std::optional<utils::Duration> m_elapsedSinceLastFired{};

  void handleReload(const float elapsedSeconds);
};

} // namespace bsgo
