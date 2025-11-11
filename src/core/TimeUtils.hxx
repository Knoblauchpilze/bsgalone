
#pragma once

#include "TimeUtils.hh"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace core {

inline auto now() noexcept -> TimeStamp
{
  return std::chrono::system_clock::now();
}

inline auto toMilliseconds(const int ms) noexcept -> Duration
{
  return Milliseconds(ms);
}

inline auto toMilliseconds(const Duration &d) noexcept -> float
{
  const auto time = std::chrono::duration_cast<Milliseconds>(d);
  const auto ms   = time.count();
  return 1.0f * ms;
}

inline auto toSeconds(const Duration &d) noexcept -> float
{
  const auto ms                       = toMilliseconds(d);
  constexpr auto MILLIS_IN_ONE_SECOND = 1000.0f;
  return 1.0f * ms / MILLIS_IN_ONE_SECOND;
}

inline auto timeToString(const TimeStamp &t) noexcept -> std::string
{
  // See here:
  // https://stackoverflow.com/questions/34857119/how-to-convert-stdchronotime-point-to-string/34858704
  // And here:
  // https://stackoverflow.com/questions/32873659/how-can-i-get-current-time-of-day-in-milliseconds-in-c/32874098#32874098
  const auto tt = std::chrono::system_clock::to_time_t(t);
  const auto tm = *std::gmtime(&tt); // GMT (UTC)

  const auto ms = std::chrono::duration_cast<Milliseconds>(t.time_since_epoch())
                  - std::chrono::duration_cast<std::chrono::seconds>(t.time_since_epoch());

  std::stringstream ss;
  ss << std::put_time(&tm, "UTC: %Y-%m-%d %H:%M:%S");
  ss << "." << ms.count() << "ms";

  return ss.str();
}

inline auto durationToMsString(const Duration &d) noexcept -> std::string
{
  // https://stackoverflow.com/questions/22590821/convert-stdduration-to-human-readable-time
  const auto s  = std::chrono::duration_cast<Milliseconds>(d);
  const auto ms = s.count();
  return std::to_string(ms) + "ms";
}

inline auto durationToPrettyString(Duration d, const bool includeFractionalSeconds) noexcept
  -> std::string
{
  // https://stackoverflow.com/questions/22590821/convert-stdduration-to-human-readable-time
  const auto h = std::chrono::duration_cast<std::chrono::hours>(d);
  d -= h;
  const auto m = std::chrono::duration_cast<std::chrono::minutes>(d);
  d -= m;
  const auto s = std::chrono::duration_cast<std::chrono::seconds>(d);
  d -= s;
  const auto ms = std::chrono::duration_cast<Milliseconds>(d);

  const auto hours   = h.count();
  const auto minutes = m.count();
  const auto seconds = s.count();
  const auto millis  = ms.count();

  std::stringstream ss;
  ss.fill('0');
  if (hours)
  {
    ss << hours << "h";
  }
  if (0 != hours || 0 != minutes)
  {
    if (hours)
    {
      ss << std::setw(2);
    }
    ss << minutes << "m";
  }
  if (0 != hours || 0 != minutes || 0 != seconds)
  {
    if (hours || minutes)
    {
      ss << std::setw(2);
    }
    ss << seconds;
    if (0 == millis || !includeFractionalSeconds)
    {
      ss << 's';
    }
  }

  if (0 != millis && includeFractionalSeconds)
  {
    if (ss.str().empty())
    {
      ss << "0";
    }
    ss << "." << std::setw(3) << millis << "ms";
  }

  if (0 == hours && 0 == minutes && 0 == seconds && (0 == millis || !includeFractionalSeconds))
  {
    return "0s";
  }

  return ss.str();
}

inline auto diffInMs(const TimeStamp &start, const TimeStamp &end) noexcept -> float
{
  const auto elapsed = end - start;
  const auto ms      = std::chrono::duration_cast<Milliseconds>(elapsed);
  return ms.count();
}

} // namespace core

inline auto operator<<(std::ostream &out, const core::TimeStamp &t) noexcept -> std::ostream &
{
  // As per this link:
  // https://stackoverflow.com/questions/36963373/saving-and-loading-stdchronotime-point-to-file
  // We can serialize a timestamp by converting it to its epoch representation.
  const auto et = t.time_since_epoch().count();
  out.write(reinterpret_cast<const char *>(&et), sizeof(et));
  return out;
}

inline auto operator>>(std::istream &in, core::TimeStamp &t) noexcept -> std::istream &
{
  std::chrono::system_clock::rep et;

  t = core::now();
  in.read(reinterpret_cast<char *>(&et), sizeof(et));
  if (in)
  {
    t = core::TimeStamp(std::chrono::system_clock::duration{et});
  }

  return in;
}
