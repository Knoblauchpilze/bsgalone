
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

auto serialize(std::ostream &out, const Eigen::Vector3f &v) -> std::ostream &
{
  out << v(0);
  out << v(1);
  out << v(2);

  return out;
}

auto deserialize(std::istream &in, Eigen::Vector3f &v) -> std::istream &
{
  in >> v(0);
  in >> v(1);
  in >> v(2);

  return in;
}

} // namespace bsgo
