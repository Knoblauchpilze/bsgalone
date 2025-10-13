
#include "SyncComponent.hh"

namespace bsgo {

namespace {
constexpr auto BASE_SYNC_INTERVAL_TICK    = 1.0f;
constexpr auto MAX_SYNC_JITTER_CENTI_TICK = 100;

auto generateJitteredSyncInterval() -> TickDuration
{
  const auto jitterPercentage = static_cast<float>(std::rand() % MAX_SYNC_JITTER_CENTI_TICK);

  const auto jitteredSync = BASE_SYNC_INTERVAL_TICK * (1.0f + jitterPercentage / 100.0f);
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
