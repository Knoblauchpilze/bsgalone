
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

void serialize(std::ostream &out, const Eigen::Vector3f &value)
{
  core::serialize(out, value(0));
  core::serialize(out, value(1));
  core::serialize(out, value(2));
}

void serialize(std::ostream &out, const std::optional<Eigen::Vector3f> &value)
{
  const auto hasValue = value.has_value();
  out.write(reinterpret_cast<const char *>(&hasValue), sizeof(bool));
  if (hasValue)
  {
    serialize(out, *value);
  }
}

bool deserialize(std::istream &in, Eigen::Vector3f &value)
{
  bool ok{true};
  ok &= core::deserialize(in, value(0));
  ok &= core::deserialize(in, value(1));
  ok &= core::deserialize(in, value(2));

  return ok;
}

bool deserialize(std::istream &in, std::optional<Eigen::Vector3f> &value)
{
  bool hasValue{false};
  in.read(reinterpret_cast<char *>(&hasValue), sizeof(bool));
  if (hasValue)
  {
    Eigen::Vector3f raw{};
    deserialize(in, raw);
    value = raw;
  }
  else
  {
    value.reset();
  }

  return in.good();
}

} // namespace bsgo
