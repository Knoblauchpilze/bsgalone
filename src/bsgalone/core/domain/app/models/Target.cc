
#include "Target.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

bool Target::operator==(const Target &rhs) const
{
  return sourceDbId == rhs.sourceDbId && sourceKind == rhs.sourceKind
         && targetDbId == rhs.targetDbId && targetKind == rhs.targetKind;
}

auto Target::str() const -> std::string
{
  std::string out = "[(" + core::str(sourceKind) + "," + core::str(sourceDbId) + "), (";

  if (targetKind)
  {
    out += core::str(*targetKind);
  }
  else
  {
    out += "none";
  }

  if (targetDbId)
  {
    out += ",";
    out += core::str(*targetDbId);
  }
  else
  {
    out += ",none";
  }

  out += ")]";

  return out;
}

auto operator<<(std::ostream &out, const Target &target) -> std::ostream &
{
  ::core::serialize(out, target.sourceDbId);
  ::core::serialize(out, target.sourceKind);
  ::core::serialize(out, target.targetDbId);
  ::core::serialize(out, target.targetKind);

  return out;
}

bool operator>>(std::istream &in, Target &target)
{
  bool ok{true};

  ok &= ::core::deserialize(in, target.sourceDbId);
  ok &= ::core::deserialize(in, target.sourceKind);
  ok &= ::core::deserialize(in, target.targetDbId);
  ok &= ::core::deserialize(in, target.targetKind);

  return ok;
}

} // namespace bsgalone::core
