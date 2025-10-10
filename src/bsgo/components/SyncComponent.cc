
#include "SyncComponent.hh"

namespace bsgo {

namespace {
constexpr auto BASE_SYNC_INTERVAL_TICK = 10;
constexpr auto MAX_SYNC_JITTER_TICK    = 1;

auto generateJitteredSyncInterval() -> TickDuration
{
  const auto jitteredSync = BASE_SYNC_INTERVAL_TICK + std::rand() % MAX_SYNC_JITTER_TICK;
  return TickDuration::fromInt(jitteredSync);
}
} // namespace

SyncComponent::SyncComponent(const ComponentType type)
  : AbstractComponent(type)
  , m_untilNextSync(generateJitteredSyncInterval())
{}

bool SyncComponent::needsSync() const
{
  return m_needsSync;
}

void SyncComponent::markForSync(const bool needsSync)
{
  m_needsSync = needsSync;
}

void SyncComponent::markAsJustSynced()
{
  markForSync(false);
  m_untilNextSync        = generateJitteredSyncInterval();
  m_elapsedSinceLastSync = TickDuration();
}

void SyncComponent::update(const TickData &data)
{
  m_elapsedSinceLastSync += data.elapsed;
  if (m_untilNextSync <= m_elapsedSinceLastSync)
  {
    m_needsSync = true;
  }
}

} // namespace bsgo
