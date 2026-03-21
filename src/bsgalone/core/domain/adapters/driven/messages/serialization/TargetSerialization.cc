
#include "TargetSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

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
