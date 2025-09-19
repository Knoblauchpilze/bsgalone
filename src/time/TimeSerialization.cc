
#include "TimeSerialization.hh"
#include "tz.h"

namespace date_time {
// https://en.wikipedia.org/wiki/List_of_tz_database_time_zones#List
// Search for the "Etc/UTC"
constexpr auto UTC_0_TIME_ZONE_NAME = "Etc/UTC";

auto fromString(const std::string /*time*/) -> std::string
{
  return "";
}

auto toString(const core::TimeStamp &time) -> std::string
{
  // https://howardhinnant.github.io/date/tz.html#current_local
  const auto zonedTime = date::make_zoned(date::locate_zone(UTC_0_TIME_ZONE_NAME),
                                          floor<core::Milliseconds>(time));

  // TODO: Correctly return the expected format.
  // The desired format is something like: 2025-09-10 07:54:04.58163+02
  // https://stackoverflow.com/questions/10289017/how-to-format-date-and-time-string-in-c
  // https://en.cppreference.com/w/cpp/io/manip/put_time
  std::stringstream out;
  out << zonedTime;

  return out.str();
}

} // namespace date_time
