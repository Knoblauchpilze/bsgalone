
#include "Target.hh"

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

} // namespace bsgalone::core
