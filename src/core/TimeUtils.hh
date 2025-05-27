
#pragma once

#include <chrono>
#include <string>

namespace core {

/// @brief - Convenience define to refer to a time point. Allows to keep track
/// of absolute timestamps to trigger some behaviors.
using TimeStamp = std::chrono::system_clock::time_point;

/// @brief - Conveniently represent a duration. It can be added or subtracted to
/// a timestamp in an easy way.
using Duration = std::chrono::system_clock::duration;

/// @brief - Convenience define to represent a single millisecond.
using Milliseconds = std::chrono::milliseconds;

/// @brief - Used to retrieve the timestamp as of now.
/// @return - a timepoint at the moment of the call to this function.
auto now() noexcept -> TimeStamp;

/// @brief - Used to convert the input integer as a time duration expressed in
/// milliseconds.
/// @param ms - the number of milliseconds to convert.
/// @return - a duration representing the input number of milliseconds.
auto toMilliseconds(const int ms) noexcept -> Duration;

/// @brief - Used to convert the input duration to the corresponding floating
/// point number of milliseconds.
/// @param d - the duration to convert.
/// @return - the number of milliseconds in the input duration.
auto toMilliseconds(const Duration &d) noexcept -> float;

/// @brief - Converts a timestamp to a human readable string.
/// @param t - the time to convert.
/// @return - a string representing this time.
auto timeToString(const TimeStamp &t) noexcept -> std::string;

/// @brief - Converts the duration to a human readable string expressed
/// in milliseconds.
/// @param d - the duration to convert to string. Be aware that it should
/// not be 'too far' from one millisecond in order to get a display that
/// is not a huge batch of numbers.
/// @return - the corresponding string.
auto durationToMsString(const Duration &d) noexcept -> std::string;

/// @brief - Convert the input duration to a human readable string similar to
/// Xh Ym Zs.
/// @param d - the duration to convert.
/// @param includeFractionalSeconds - whether or not milliseconds should be
/// included in the output string.
/// @return - a string representing this duration.
auto durationToPrettyString(Duration d, const bool includeFractionalSeconds = false) noexcept
  -> std::string;

/// @brief - Return the difference in milliseconds between the two input
/// timestamps using a float value.
/// @param start - the start of the time interval.
/// @param end - the end of the time interval.
/// @return - a float value for the interval in milliseconds.
auto diffInMs(const TimeStamp &start, const TimeStamp &end) noexcept -> float;

} // namespace core

/// @brief - Serialization function allowing to insert the representation of the
/// `TimeStamp` object to the stream.
/// @param out - the stream into which the `TimeStamp` will be appended.
/// @param rng - the `TimeStamp` object to serialize.
/// @return - the modified stream.
auto operator<<(std::ostream &out, const core::TimeStamp &t) noexcept -> std::ostream &;

/// @brief - Deserialization function allowing to extract the representation of
/// a `TimeStamp` object from the stream.
/// @param in - the stream from which the timestamp will be generated.
/// @param t - the timestamp object into which the deserialization is performed.
/// @return - the modified stream.
auto operator>>(std::istream &in, core::TimeStamp &t) noexcept -> std::istream &;

#include "TimeUtils.hxx"
