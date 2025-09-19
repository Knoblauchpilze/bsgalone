
#pragma once

#include "TimeUtils.hh"
#include <string>

namespace date_time {

auto fromString(const std::string time) -> std::string;
auto toString(const core::TimeStamp &time) -> std::string;

} // namespace date_time
