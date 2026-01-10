
#pragma once

#include <cstdint>
#include <string>

namespace net {

using MessageId = std::uint64_t;

auto str(const MessageId id) -> std::string;

} // namespace net
