
#pragma once

#include <string>

namespace bsgo {

/// @brief - Represents a duration in the game. Such a duration does not have a unit,
/// it merely represents the time in the game. It is consistent with the `Tick` class
/// as the addition between a `Tick` and a `TickDuration` is well defined.
/// A duration cannot be negative: this is enforced when constructing a new instance
/// of the class.
class TickDuration
{
  public:
  /// @brief - Creates a new zero duration.
  explicit TickDuration() = default;

  /// @brief - Creates a new duration from the input floating point value. The value
  /// cannot be negative: if this is the case an exception will be raised.
  /// @param elapsed - the duration to represent.
  explicit TickDuration(const float elapsed);

  auto str() const -> std::string;

  /// TODO: This should be removed.
  /// @brief - Temporary method to convert this duration to real seconds. This
  /// is intended as a workaround while the core game logic is migrated to use
  /// Ticks and TickDuration instead of core::Duration.
  /// This method assumes that a `Tick` is equivalent to 100ms.
  /// @return - the duration expressed in seconds.
  auto toSeconds() const -> float;

  private:
  float m_elapsed{0.0f};

  void validate();
};

} // namespace bsgo
