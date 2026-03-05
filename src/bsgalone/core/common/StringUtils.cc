
#include "StringUtils.hh"
#include <algorithm>
#include <iomanip>
#include <sstream>

namespace bsgo {

auto floatToStr(const float value, const int decimals) -> std::string
{
  std::stringstream ss;
  ss << std::fixed << std::setprecision(decimals) << value;
  return ss.str();
}

auto floatToSignedStr(const float value, const int decimals) -> std::string
{
  std::stringstream ss;
  ss << std::fixed << std::showpos << std::setprecision(decimals) << value;
  return ss.str();
}

auto capitalizeString(const std::string &s, const bool onlyFirstLetter) -> std::string
{
  std::string out(s);

  if (onlyFirstLetter)
  {
    out[0] = std::toupper(out[0]);
    return out;
  }

  std::transform(out.begin(), out.end(), out.begin(), [](const char c) { return std::toupper(c); });

  return out;
}

} // namespace bsgo
