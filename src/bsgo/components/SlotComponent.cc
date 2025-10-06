
#include "SlotComponent.hh"

namespace bsgo {

SlotComponent::SlotComponent(const ComponentType &type, const SlotComponentData &data)
  : AbstractComponent(type)
  , m_dbId(data.dbId)
  , m_offensive(data.offensive)
  , m_powerCost(data.powerCost)
  , m_range(data.range)
  , m_reloadTime(data.reloadTime)
{
  addModule("slot");
}

void SlotComponent::update(const TickData &data)
{
  handleReload(data);
}

auto SlotComponent::dbId() const -> Uuid
{
  return m_dbId;
}

bool SlotComponent::isOffensive() const
{
  return m_offensive;
}

auto SlotComponent::powerCost() const -> float
{
  return m_powerCost;
}

auto SlotComponent::maybeRange() const -> std::optional<float>
{
  return m_range;
}

auto SlotComponent::range() const -> float
{
  if (!m_range)
  {
    error("Expected component to have a range");
  }
  return *m_range;
}

auto SlotComponent::firingState() const noexcept -> FiringState
{
  return m_firingState;
}

bool SlotComponent::canFire() const noexcept
{
  return FiringState::READY == firingState();
}

bool SlotComponent::isReloading() const noexcept
{
  return m_elapsedSinceLastFired.has_value();
}

auto SlotComponent::reloadPercentage() const -> float
{
  if (!isReloading())
  {
    return 1.0f;
  }

  return *m_elapsedSinceLastFired / m_reloadTime;
}

auto SlotComponent::elapsedSinceLastFired() const -> std::optional<core::Duration>
{
  // TODO: We should not convert to real time here
  constexpr auto MILLIS_IN_ONE_SECOND = 1000.0f;
  return core::toMilliseconds(MILLIS_IN_ONE_SECOND * m_elapsedSinceLastFired->toSeconds());
}

void SlotComponent::setFiringState(const FiringState &firingState)
{
  m_firingState = firingState;
}

void SlotComponent::registerFireRequest()
{
  m_fireRequest = true;
}

bool SlotComponent::hasFireRequest() const
{
  return m_fireRequest;
}

void SlotComponent::fire()
{
  clearFireRequest();
  if (!canFire())
  {
    error("Failed to use slot", "Still reloading");
  }

  m_elapsedSinceLastFired = TickDuration();
}

void SlotComponent::clearFireRequest()
{
  m_fireRequest = false;
}

void SlotComponent::handleReload(const TickData &data)
{
  if (!m_elapsedSinceLastFired)
  {
    return;
  }

  (*m_elapsedSinceLastFired) += data.elapsed;

  if (*m_elapsedSinceLastFired >= m_reloadTime)
  {
    m_elapsedSinceLastFired.reset();
  }
}

} // namespace bsgo
