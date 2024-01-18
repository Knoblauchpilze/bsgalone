
#include "VectorUtils.hh"
#include "StringUtils.hh"
#include <core_utils/SerializationUtils.hh>

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

auto serialize(std::ostream &out, const Eigen::Vector3f &v) -> std::ostream &
{
  utils::serialize(out, v(0));
  utils::serialize(out, v(1));
  utils::serialize(out, v(2));

  return out;
}

bool deserialize(std::istream &in, Eigen::Vector3f &v)
{
  bool ok{true};
  ok &= utils::deserialize(in, v(0));
  ok &= utils::deserialize(in, v(1));
  ok &= utils::deserialize(in, v(2));

  return ok;
}

} // namespace bsgo
