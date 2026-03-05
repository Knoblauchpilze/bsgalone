
#include "VectorUtils.hh"
#include "SerializationUtils.hh"
#include "StringUtils.hh"

namespace bsgo {

auto str(const Eigen::Vector3f &v) -> std::string
{
  std::string out;
  out += floatToStr(v(0), 3);
  out += "x";
  out += floatToStr(v(1), 3);
  out += "x";
  out += floatToStr(v(2), 3);

  return out;
}

auto operator*(const Eigen::Vector3f &lhs, const chrono::TickDuration &rhs) -> Eigen::Vector3f
{
  return Eigen::Vector3f(lhs(0) * rhs, lhs(1) * rhs, lhs(2) * rhs);
}

} // namespace bsgo
