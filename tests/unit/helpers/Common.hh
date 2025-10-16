
#pragma once

#include <utility>

namespace test {

template<typename T>
auto serializeAndDeserialize(const T &value) -> T;

template<typename T>
auto serializeAndDeserialize(const T &expected, const bool truncate) -> std::pair<bool, T>;

} // namespace test

#include "Common.hxx"
