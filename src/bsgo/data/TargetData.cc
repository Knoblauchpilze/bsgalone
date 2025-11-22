
#include "TargetData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool TargetData::operator==(const TargetData &rhs) const
{
  return sourceDbId == rhs.sourceDbId && sourceKind == rhs.sourceKind
         && targetDbId == rhs.targetDbId && targetKind == rhs.targetKind;
}

auto TargetData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, sourceDbId);
  core::serialize(out, sourceKind);
  core::serialize(out, targetDbId);
  core::serialize(out, targetKind);

  return out;
}

bool TargetData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, sourceDbId);
  ok &= core::deserialize(in, sourceKind);
  ok &= core::deserialize(in, targetDbId);
  ok &= core::deserialize(in, targetKind);

  return ok;
}

} // namespace bsgo
