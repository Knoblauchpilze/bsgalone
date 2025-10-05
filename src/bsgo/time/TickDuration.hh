
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

  /// @brief - Compares two durations and returns true if they are equal. This class
  /// defines a built-in tolerance that can be accessed through `TickDuration::TOLERANCE`.
  /// @param rhs - the duration to compare
  /// @return - true in case both durations are equal within the tolerance
  bool operator==(const TickDuration &rhs) const;

  bool operator<(const TickDuration &rhs) const;
  bool operator<=(const TickDuration &rhs) const;
  bool operator>(const TickDuration &rhs) const;
  bool operator>=(const TickDuration &rhs) const;

  /// TODO: This should be removed.
  /// @brief - Temporary method to convert this duration to real seconds. This
  /// is intended as a workaround while the core game logic is migrated to use
  /// Ticks and TickDuration instead of core::Duration.
  /// This method assumes that a `Tick` is equivalent to 100ms.
  /// @return - the duration expressed in seconds.
  auto toSeconds() const -> float;

  /// @brief - Tolerance used when comparing durations. Any difference smaller than this
  /// threshold will not register as a meaningful difference.
  static const float TOLERANCE;

  private:
  float m_elapsed{0.0f};

  // This construction allows to keep the TickDuration as an opaque type (i.e. there's
  // no way to access the inner `m_elapsed` duration) but still allow to convert in the
  // `Tick` class.
  friend class Tick;

  void validate();
};

} // namespace bsgo
