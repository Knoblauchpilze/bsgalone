
#pragma once

#include <utility>

namespace test {

template<typename T>
inline bool serializeAndDeserialize(const T &value, T &output);

template<typename T>
auto serializeAndDeserialize(const T &expected, const bool truncate) -> std::pair<bool, T>;

} // namespace test

#include "Common.hxx"
