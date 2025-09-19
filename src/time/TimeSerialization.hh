
#pragma once

#include "TimeUtils.hh"
#include <string>

namespace date_time {

auto fromString(const std::string time) -> std::string;

using UtcTimeStamp = std::chrono::utc_clock::time_point;

auto toString(const UtcTimeStamp &time) -> std::string;

} // namespace date_time
