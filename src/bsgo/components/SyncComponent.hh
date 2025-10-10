

#pragma once

#include "AbstractComponent.hh"

namespace bsgo {

class SyncComponent : public AbstractComponent
{
  public:
  SyncComponent(const ComponentType type);
  ~SyncComponent() override = default;

  bool needsSync() const;
  void markForSync(const bool needsSync = true);
  void markAsJustSynced();

  void update(const TickData &data) override;

  private:
  bool m_needsSync{false};
  TickDuration m_untilNextSync{};
  TickDuration m_elapsedSinceLastSync{};
};

} // namespace bsgo
