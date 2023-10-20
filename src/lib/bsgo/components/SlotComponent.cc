
#include "SlotComponent.hh"

namespace bsgo {

SlotComponent::SlotComponent(const std::string &name, const SlotComponentData &data)
  : IComponent(name)
  , m_powerCost(data.powerCost)
  , m_range(data.range)
  , m_reloadTime(data.reloadTime)
{
  addModule("slot");
}

void SlotComponent::update(const float elapsedSeconds)
{
  handleReload(elapsedSeconds);
}

auto SlotComponent::powerCost() const -> float
{
  return m_powerCost;
}

auto SlotComponent::range() const -> float
{
  return m_range;
}

bool SlotComponent::canFire() const noexcept
{
  return !m_elapsedSinceLastFired;
}

auto SlotComponent::reloadPercentage() const -> float
{
  if (canFire())
  {
    return 1.0f;
  }

  // https://stackoverflow.com/questions/76522118/dividing-two-chronodurations-to-get-fraction
  const auto reloadAsFloat = std::chrono::duration<float, std::milli>(m_reloadTime);
  return *m_elapsedSinceLastFired / reloadAsFloat;
}

void SlotComponent::fire()
{
  if (!canFire())
  {
    error("Failed to use slot", "Still reloading");
  }

  m_elapsedSinceLastFired = utils::Duration(0);
}

void SlotComponent::handleReload(const float elapsedSeconds)
{
  if (!m_elapsedSinceLastFired)
  {
    return;
  }

  constexpr auto MILLISECONDS_IN_A_SECONDS = 1000;
  (*m_elapsedSinceLastFired) += utils::Milliseconds(
    static_cast<int>(elapsedSeconds * MILLISECONDS_IN_A_SECONDS));

  if (*m_elapsedSinceLastFired > m_reloadTime)
  {
    m_elapsedSinceLastFired.reset();
  }
}

} // namespace bsgo
