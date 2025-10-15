
#pragma once

#include <string>

namespace chrono {

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

  /// @brief - Adds duration to this tick and returns a reference to this.
  /// @param duration - the duration to add
  /// @return - a reference to this
  auto operator+=(const TickDuration &duration) -> TickDuration &;

  /// @brief - Performs the division of this by duration and returns the result as
  /// a floating point value. The return value can be understood as a ratio between
  /// the two input duration: it is a dimensionless value.
  /// @param duration - the duration to divide this by
  /// @return - the result of this divided by duration as a dimensionless unit
  auto operator/(const TickDuration &duration) const -> float;

  /// @brief - Multiplies rhs with this and returns the result as a floating point value.
  /// The result is a dimensionless expression: the implicit assumption is that rhs is a value
  /// with a unit of Tick^-1, meaning its a quantity per Tick (e.g. m/Tick).
  /// @param rhs - a floating point value
  /// @return - the result of multiplying this with rhs
  auto operator*(const float rhs) const -> float;

  /// @brief - Subtracts rhs from this duration. In case rhs is greater than this duration
  /// the return value is equivalent to calling TickDuration().
  /// @param rhs - the duration to subtract
  /// @return - the result of this - rhs or TickDuration in case rhs is greater than this
  auto operator-(const TickDuration &rhs) const -> TickDuration;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);

  /// @brief - Creates a duration from an integer. This overload is to make it very
  /// intentional when a duration should be created from an integer rather than rely
  /// on the implicit conversion to floating point value.
  /// @param duration - the duration as an integer
  /// @return - the duration
  static auto fromInt(const int duration) -> TickDuration;

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

/// @brief - Multiplies lhs with rhs and returns the result as a floating point value.
/// This function internally calls the operator* defined in the TickDuration class.
/// @param lhs - a floating point value
/// @param rhs - a tick duration
/// @return - the result of lhs * rhs as a float
auto operator*(const float lhs, const TickDuration &rhs) -> float;

} // namespace chrono
