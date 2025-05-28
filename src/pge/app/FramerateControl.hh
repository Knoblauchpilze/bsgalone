
#pragma once

#include "CoreObject.hh"
#include "TimeUtils.hh"
#include <optional>

namespace pge {

class FramerateControl : public core::CoreObject
{
  public:
  FramerateControl(int maxFps);

  /// @brief - Used to throttle the calling thread to maintain the maximum
  /// desired fps. This method may block the thread for a certain duration.
  void throttle();

  private:
  /// @brief - The maximum desired framerate;
  int m_maxFps;

  /// @brief - The maximum duration that each frame should take to maintain
  /// the desired maximum framerate in milliseconds.
  core::Duration m_sleepTimeInMs;

  /// @brief - A minimum duration to still consider that it's worth it to
  /// throttle the calling thread. If the remaining duration to wait is
  /// smaller than this we consider that it's a close enough approximation
  /// of the maximum framerate.
  core::Duration m_minSleepTimeInMs;

  /// @brief - The drift accumulated by not sleeping for too short duration.
  core::Duration m_sleepDriftInMs{};

  /// @brief - The last time point we have where we applied throttling. If
  /// empty we never applied throttling.
  std::optional<core::TimeStamp> m_lastThrottlingTime{};
};

} // namespace pge
