
#include "FramerateControl.hh"
#include <thread>

namespace pge {

constexpr auto MIN_SLEEP_TIME           = core::Milliseconds(5);
constexpr auto MILLISECONDS_IN_A_SECOND = 1'000;

FramerateControl::FramerateControl(int maxFps)
  : core::CoreObject("control")
  , m_maxFps(maxFps)
{
  setService("fps");
  m_sleepTimeInMs    = core::Milliseconds(MILLISECONDS_IN_A_SECOND / maxFps);
  m_minSleepTimeInMs = MIN_SLEEP_TIME;
}

void FramerateControl::throttle()
{
  if (!m_lastThrottlingTime)
  {
    m_lastThrottlingTime = core::now();
    return;
  }

  const auto now     = core::now();
  const auto elapsed = now - *m_lastThrottlingTime;

  if (elapsed > m_sleepTimeInMs)
  {
    m_lastThrottlingTime = now;
    m_sleepDriftInMs     = core::Milliseconds(0);
    return;
  }

  const auto sleepTime         = m_sleepTimeInMs - elapsed;
  const auto sleepTimeAndDrift = sleepTime + m_sleepDriftInMs;

  if (sleepTimeAndDrift < m_minSleepTimeInMs)
  {
    m_lastThrottlingTime = now;
    m_sleepDriftInMs += sleepTime;
    return;
  }

  const auto actualSleepTime   = std::min(m_sleepTimeInMs, sleepTimeAndDrift);
  const auto leftoverSleepTime = sleepTimeAndDrift - actualSleepTime;

  std::this_thread::sleep_for(sleepTime);

  m_lastThrottlingTime = core::now();
  m_sleepDriftInMs     = leftoverSleepTime;
}

} // namespace pge
