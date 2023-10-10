
#include "VectorUtils.hh"
#include "StringUtils.hh"

namespace bsgo {

auto str(const Eigen::Vector3f &v) -> std::string
{
  std::string out;
  out += pge::floatToStr(v(0), 3);
  out += "x";
  out += pge::floatToStr(v(1), 3);
  out += "x";
  out += pge::floatToStr(v(2), 3);

  return out;
}

} // namespace bsgo
