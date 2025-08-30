
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

} // namespace bsgo
