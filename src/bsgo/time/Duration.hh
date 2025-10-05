
#pragma once

#include <string>

namespace bsgo {

/// @brief - Represents a duration in the game. Such a duration does not have a unit,
/// it merely represents the time in the game. It is consistent with the `Tick` class
/// as the addition between a `Tick` and a `Duration` is well defined.
/// A duration cannot be negative: this is enforced when constructing a new instance
/// of the class.
class Duration
{
  public:
  /// @brief - Creates a new zero duration.
  explicit Duration() = default;

  /// @brief - Creates a new duration from the input floating point value. The value
  /// cannot be negative: if this is the case an exception will be raised.
  /// @param elapsed - the duration to represent.
  explicit Duration(const float elapsed);

  auto str() const -> std::string;

  private:
  float m_elapsed{0.0f};

  void validate();
};

} // namespace bsgo
