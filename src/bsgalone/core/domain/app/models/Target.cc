
#include "Target.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

bool Target::operator==(const Target &rhs) const
{
  return sourceDbId == rhs.sourceDbId && sourceKind == rhs.sourceKind
         && targetDbId == rhs.targetDbId && targetKind == rhs.targetKind;
}

auto Target::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, sourceDbId);
  ::core::serialize(out, sourceKind);
  ::core::serialize(out, targetDbId);
  ::core::serialize(out, targetKind);

  return out;
}

bool Target::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= ::core::deserialize(in, sourceDbId);
  ok &= ::core::deserialize(in, sourceKind);
  ok &= ::core::deserialize(in, targetDbId);
  ok &= ::core::deserialize(in, targetKind);

  return ok;
}

auto Target::str() const -> std::string
{
  std::string out = "[(" + core::str(sourceKind) + "," + bsgo::str(sourceDbId) + "), (";

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
    out += bsgo::str(*targetDbId);
  }
  else
  {
    out += ",none";
  }

  out += ")]";

  return out;
}

} // namespace bsgalone::core
