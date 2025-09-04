

#pragma once

#include "AbstractComponent.hh"
#include "TimeUtils.hh"

namespace bsgo {

class SyncComponent : public AbstractComponent
{
  public:
  SyncComponent(const ComponentType type);
  ~SyncComponent() override = default;

  bool needsSync() const;
  void markForSync(const bool needsSync = true);
  void markAsJustSynced();

  void update(const float elapsedSeconds) override;

  private:
  bool m_needsSync{false};
  core::Duration m_remainingUntilNextSync{};
};

} // namespace bsgo
