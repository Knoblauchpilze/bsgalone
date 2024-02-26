
#pragma once

#include <string>

namespace bsgo {

auto floatToStr(const float value, const int decimals = 2) -> std::string;
auto floatToSignedStr(const float value, const int decimals = 2) -> std::string;

auto capitalizeString(const std::string &s, const bool onlyFirstLetter = false) -> std::string;

} // namespace bsgo
