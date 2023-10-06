
#include "StringUtils.hh"
#include <iomanip>
#include <sstream>

namespace pge {

auto floatToStr(const float value, const int decimals) -> std::string
{
  std::stringstream ss;
  ss << std::fixed << std::setprecision(decimals) << value;
  return ss.str();
}

} // namespace pge
