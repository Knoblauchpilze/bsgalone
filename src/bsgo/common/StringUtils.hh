
#pragma once

#include <string>

namespace pge {

auto floatToStr(const float value, const int decimals = 2) -> std::string;
auto floatToSignedStr(const float value, const int decimals = 2) -> std::string;

} // namespace pge
